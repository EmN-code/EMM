
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/sys/printk.h>
#include <zephyr/random/random.h>
#include <string.h>
#include <time.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_SIZE 4
#define MQTT_KEEPALIVE 60
#define MQTT_QOS 0
#define MQTT_RETAIN 0

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
    struct sockaddr_in broker;
    struct mqtt_connect_param conn_param;
    struct mqtt_publish_param pub_param;
    struct mqtt_topic topic;
    struct mqtt_utf8 client_id;
    struct mqtt_utf8 payload;

    memset(&client, 0, sizeof(client));
    memset(&broker, 0, sizeof(broker));
    memset(&conn_param, 0, sizeof(conn_param));
    memset(&pub_param, 0, sizeof(pub_param));
    memset(&topic, 0, sizeof(topic));
    memset(&client_id, 0, sizeof(client_id));
    memset(&payload, 0, sizeof(payload));

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.keepalive = MQTT_KEEPALIVE;
    client.clean_session = 1;

    broker.sin_family = AF_INET;
    broker.sin_addr.s_addr = inet_addr(MQTT_BROKER_ADDR);
    broker.sin_port = htons(MQTT_BROKER_PORT);

    client_id.utf8 = "esp32_client";
    client_id.size = strlen(client_id.utf8);
    client.client_id = &client_id;

    conn_param.client_id = &client_id;
    conn_param.clean_session = 1;
    conn_param.keepalive = MQTT_KEEPALIVE;

    mqtt_client_init(&client);
    mqtt_connect(&client, &conn_param);

    topic.topic.utf8 = MQTT_TOPIC;
    topic.topic.size = strlen(MQTT_TOPIC);
    topic.qos = MQTT_QOS;

    payload.utf8 = MQTT_PAYLOAD;
    payload.size = MQTT_PAYLOAD_SIZE;

    pub_param.message.topic = &topic;
    pub_param.message.payload = &payload;
    pub_param.message.payload_len = MQTT_PAYLOAD_SIZE;
    pub_param.message.dup_flag = 0;
    pub_param.message.retain_flag = MQTT_RETAIN;
    pub_param.message_id = sys_rand32_get();

    while (1) {
        mqtt_publish(&client, &pub_param);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
