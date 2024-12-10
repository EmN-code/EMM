
#include <zephyr/kernel.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/sys/printk.h>
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_SIZE 4
#define MQTT_KEEPALIVE_SEC 60
#define MQTT_CLEAN_SESSION 1
#define MQTT_QOS 0

struct mqtt_client client;
uint8_t rx_buffer[100];
uint8_t tx_buffer[100];

void mqtt_evt_handler(struct mqtt_client *const c,
                      const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        if (evt->result != 0) {
            printk("Connection failed %d\n", evt->result);
        } else {
            printk("Connected and ready to publish\n");
        }
        break;
    case MQTT_EVT_DISCONNECT:
        printk("Disconnected\n");
        break;
    default:
        break;
    }
}

void publish_state(struct mqtt_client *client)
{
    struct mqtt_publish_param param;
    struct mqtt_topic topic = {
        .topic.utf8 = MQTT_TOPIC,
        .topic.size = strlen(MQTT_TOPIC),
    };
    struct mqtt_binstr payload = {
        .data = MQTT_PAYLOAD,
        .len = MQTT_PAYLOAD_SIZE,
    };

    param.message.topic = topic;
    param.message.payload = payload;
    param.message_id = 0;
    param.dup_flag = 0;
    param.retain_flag = 0;

    int ret = mqtt_publish(client, &param);
    if (ret) {
        printk("Failed to publish, error: %d\n", ret);
    } else {
        printk("State published\n");
    }
}

int main(void)
{
    int sock;
    struct sockaddr_in broker;

    memset(&client, 0, sizeof(client));
    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.keepalive = MQTT_KEEPALIVE_SEC;
    client.clean_session = MQTT_CLEAN_SESSION;

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    if (connect(sock, (struct sockaddr *)&broker, sizeof(broker)) < 0) {
        printk("Failed to connect to broker\n");
        close(sock);
        return -1;
    }

    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.transport.tcp.sock = sock;

    if (mqtt_connect(&client) != 0) {
        printk("MQTT connect failed\n");
        close(sock);
        return -1;
    }

    while (1) {
        k_sleep(K_SECONDS(5));
        publish_state(&client);
    }

    return 0;
}
