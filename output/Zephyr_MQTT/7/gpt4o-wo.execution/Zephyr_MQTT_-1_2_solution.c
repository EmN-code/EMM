
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_LEN 4
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_DUP 0

static struct mqtt_client client;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;
static struct mqtt_publish_message publish_message;
static struct sockaddr_in broker;

void mqtt_evt_handler(struct mqtt_client *const c,
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

void publish_state(struct mqtt_client *client)
{
    publish_message.topic.utf8 = MQTT_TOPIC;
    publish_message.topic.size = strlen(MQTT_TOPIC);
    publish_message.payload.data = MQTT_PAYLOAD;
    publish_message.payload.len = MQTT_PAYLOAD_LEN;

    publish_param.message = publish_message;
    publish_param.message_id = 0;
    publish_param.dup_flag = MQTT_DUP;
    publish_param.retain_flag = MQTT_RETAIN;

    int ret = mqtt_publish(client, &publish_param);
    if (ret) {
        printk("Failed to publish message: %d\n", ret);
    } else {
        printk("Message published\n");
    }
}

int main(void)
{
    int sock;
    int ret;

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printk("Failed to create socket: %d\n", sock);
        return -1;
    }

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    ret = connect(sock, (struct sockaddr *)&broker, sizeof(broker));
    if (ret < 0) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        close(sock);
        return -1;
    }

    mqtt_client_init(&client);
    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = "zephyr_client";
    client.client_id.size = strlen("zephyr_client");
    client.password = NULL;
    client.user_name = NULL;
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.clean_session = 1;
    client.keepalive = 60;
    client.will_topic = NULL;
    client.will_msg = NULL;
    client.will_qos = 0;
    client.will_retain = 0;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    ret = mqtt_connect(&client);
    if (ret) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        close(sock);
        return -1;
    }

    while (1) {
        publish_state(&client);
        k_sleep(K_SECONDS(5));
    }

    mqtt_disconnect(&client);
    close(sock);
    return 0;
}
