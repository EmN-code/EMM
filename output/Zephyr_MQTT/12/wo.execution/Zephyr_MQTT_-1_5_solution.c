
#include <zephyr/kernel.h>
#include <time.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

struct mqtt_client client;
struct mqtt_publish_param publish_param;
struct mqtt_publish_message publish_message;
struct mqtt_conn_param conn_param;
struct sockaddr_storage broker;

void mqtt_event_handler(struct mqtt_client *const client,
                        const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            if (evt->result == 0) {
                printk("Connected to MQTT broker\n");
                publish_param.message = publish_message;
                publish_param.message_id = 1;
                publish_param.dup_flag = 0;
                publish_param.retain_flag = 0;
                publish_message.topic.utf8 = MQTT_TOPIC;
                publish_message.topic.size = strlen(MQTT_TOPIC);
                publish_message.payload.data = MQTT_PAYLOAD;
                publish_message.payload.len = strlen(MQTT_PAYLOAD);
                mqtt_publish(&client, &publish_param);
            } else {
                printk("Failed to connect to MQTT broker: %d\n", evt->result);
            }
            break;
        case MQTT_EVT_PUBACK:
            if (evt->result == 0) {
                printk("Message published\n");
            } else {
                printk("Failed to publish message: %d\n", evt->result);
            }
            break;
        default:
            break;
    }
}

void publish_state(struct k_work *work)
{
    static int state = 0;
    const char *states[] = {"work", "not work"};

    publish_message.payload.data = states[state];
    publish_message.payload.len = strlen(states[state]);
    mqtt_publish(&client, &publish_param);

    state = !state;
}

K_WORK_DEFINE(publish_work, publish_state);

int main(void)
{
    int sock;
    struct sockaddr_in *broker_addr = (struct sockaddr_in *)&broker;

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    broker_addr->sin_family = AF_INET;
    broker_addr->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker_addr->sin_addr);

    if (connect(sock, (struct sockaddr *)broker_addr, sizeof(*broker_addr)) < 0) {
        printk("Failed to connect to MQTT broker\n");
        close(sock);
        return -1;
    }

    mqtt_client_init(&client);
    client.broker = &broker;
    client.broker_len = sizeof(broker);
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.transport.tcp.sock = sock;
    client.evt_cb = mqtt_event_handler;

    conn_param.keepalive = 60;
    conn_param.clean_session = 1;
    conn_param.client_id.utf8 = "zephyr_client";
    conn_param.client_id.size = strlen("zephyr_client");

    if (mqtt_connect(&client, &conn_param) < 0) {
        printk("Failed to connect to MQTT broker\n");
        close(sock);
        return -1;
    }

    while (1) {
        k_sleep(K_SECONDS(5));
        k_work_submit(&publish_work);
    }

    return 0;
}
