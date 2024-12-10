
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
#define MQTT_PAYLOAD_LEN 4
#define MQTT_KEEPALIVE 60
#define MQTT_CLEAN_SESSION 1

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
    struct mqtt_publish_message pub_msg;

    memset(&client, 0, sizeof(client));
    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.keepalive = MQTT_KEEPALIVE;
    client.clean_session = MQTT_CLEAN_SESSION;

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    conn_param.client_id.utf8 = "zephyr_client";
    conn_param.client_id.size = strlen(conn_param.client_id.utf8);
    conn_param.user_name = NULL;
    conn_param.password = NULL;
    conn_param.keepalive = MQTT_KEEPALIVE;
    conn_param.clean_session = MQTT_CLEAN_SESSION;
    conn_param.will_topic = NULL;
    conn_param.will_message = NULL;

    mqtt_connect(&client, &conn_param);

    pub_msg.topic.utf8 = MQTT_TOPIC;
    pub_msg.topic.size = strlen(pub_msg.topic.utf8);
    pub_msg.payload.data = MQTT_PAYLOAD;
    pub_msg.payload.len = MQTT_PAYLOAD_LEN;
    pub_msg.qos = MQTT_QOS_0_AT_MOST_ONCE;
    pub_msg.retain = 0;

    pub_param.message = pub_msg;
    pub_param.message_id = 0;
    pub_param.dup_flag = 0;
    pub_param.retain_flag = 0;

    while (1) {
        mqtt_publish(&client, &pub_param);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
