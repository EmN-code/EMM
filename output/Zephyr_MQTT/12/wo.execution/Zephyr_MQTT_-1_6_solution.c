
#include <zephyr/kernel.h>
#include <time.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

static struct mqtt_client client;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;
static struct mqtt_connect_param connect_param;
static struct mqtt_conn_param conn_param;

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
    static uint16_t message_id = 0;
    struct mqtt_publish_param param;
    struct mqtt_publish_message message;

    message.topic.utf8 = MQTT_TOPIC;
    message.topic.size = strlen(MQTT_TOPIC);
    message.payload.data = MQTT_PAYLOAD;
    message.payload.len = strlen(MQTT_PAYLOAD);
    message.qos = MQTT_QOS_0_AT_MOST_ONCE;

    param.message = message;
    param.message_id = message_id++;
    param.dup_flag = 0;
    param.retain_flag = 0;

    mqtt_publish(client, &param);
}

int main(void)
{
    int ret;
    struct sockaddr_in broker;

    memset(&client, 0, sizeof(client));
    memset(&broker, 0, sizeof(broker));

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = "zephyr_client";
    client.client_id.size = strlen("zephyr_client");
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.transport.tcp.rx_buf = rx_buffer;
    client.transport.tcp.rx_buf_size = sizeof(rx_buffer);
    client.transport.tcp.tx_buf = tx_buffer;
    client.transport.tcp.tx_buf_size = sizeof(tx_buffer);

    connect_param.keepalive = 60;
    connect_param.clean_session = 1;
    connect_param.client_id = client.client_id;
    connect_param.user_name.utf8 = NULL;
    connect_param.user_name.size = 0;
    connect_param.password.utf8 = NULL;
    connect_param.password.size = 0;

    conn_param.client = &client;
    conn_param.connect_param = &connect_param;

    ret = mqtt_connect(&client);
    if (ret) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        return ret;
    }

    while (1) {
        k_sleep(K_SECONDS(5));
        publish_state(&client);
    }

    return 0;
}
