
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_if.h>
#include <zephyr/sys/printk.h>
#include <zephyr/random/random.h>
#include <inet_pton.h>
#include <string.h>
#include <time.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_LEN 4
#define MQTT_KEEPALIVE 60
#define MQTT_CLEAN_SESSION 1
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_DUP 0

struct mqtt_client client;
uint8_t rx_buffer[100];
uint8_t tx_buffer[100];

void mqtt_evt_handler(struct mqtt_client *const client,
                      const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printk("Connected to MQTT broker\n");
            break;
        case MQTT_EVT_DISCONNECT:
            printk("Disconnected from MQTT broker\n");
            break;
        default:
            break;
    }
}

int main(void)
{
    struct sockaddr_storage broker;
    struct sockaddr_in *broker_addr = (struct sockaddr_in *)&broker;
    int sock;
    int ret;

    memset(&broker, 0, sizeof(broker));
    broker_addr->sin_family = AF_INET;
    broker_addr->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker_addr->sin_addr);

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    ret = connect(sock, (struct sockaddr *)broker_addr, sizeof(broker));
    if (ret < 0) {
        printk("Failed to connect to MQTT broker\n");
        close(sock);
        return -1;
    }

    mqtt_client_init(&client);
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.transport.tcp.sock = sock;
    client.client_id.utf8 = (uint8_t *)"zephyr_client";
    client.client_id.size = strlen((char *)client.client_id.utf8);
    client.evt_cb = mqtt_evt_handler;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.keepalive = MQTT_KEEPALIVE;
    client.clean_session = MQTT_CLEAN_SESSION;

    ret = mqtt_connect(&client);
    if (ret < 0) {
        printk("Failed to connect to MQTT broker\n");
        close(sock);
        return -1;
    }

    while (1) {
        struct mqtt_publish_param param;
        param.message.topic.utf8 = (uint8_t *)MQTT_TOPIC;
        param.message.topic.size = strlen(MQTT_TOPIC);
        param.message.payload.data = (uint8_t *)MQTT_PAYLOAD;
        param.message.payload.len = MQTT_PAYLOAD_LEN;
        param.message.qos = MQTT_QOS;
        param.message.retained = MQTT_RETAIN;
        param.dup_flag = MQTT_DUP;
        param.message_id = sys_rand32_get();

        ret = mqtt_publish(&client, &param);
        if (ret < 0) {
            printk("Failed to publish message\n");
        } else {
            printk("Message published\n");
        }

        k_sleep(K_SECONDS(5));
    }

    mqtt_disconnect(&client);
    close(sock);
    return 0;
}
