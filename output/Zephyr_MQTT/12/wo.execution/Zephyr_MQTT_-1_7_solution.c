
#include <zephyr/kernel.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/net_core.h>
#include <zephyr/net/net_event.h>
#include <zephyr/net/net_context.h>
#include <zephyr/net/net_pkt.h>
#include <zephyr/net/net_l2.h>
#include <zephyr/net/net_stats.h>
#include <zephyr/net/net_timeout.h>
#include <zephyr/net/net_ip.h>
#include <time.h>
#include <string.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_SIZE 4
#define MQTT_QOS 0

static struct mqtt_client client;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
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
    struct mqtt_publish_param param;
    struct mqtt_publish_message message;

    message.topic.utf8 = MQTT_TOPIC;
    message.topic.size = strlen(MQTT_TOPIC);
    message.payload.data = MQTT_PAYLOAD;
    message.payload.len = MQTT_PAYLOAD_SIZE;
    message.qos = MQTT_QOS;

    param.message = message;
    param.message_id = 0;
    param.dup_flag = 0;
    param.retain_flag = 0;

    mqtt_publish(client, &param);
}

void main_thread(void)
{
    int rc;

    memset(&client, 0, sizeof(client));
    memset(&connect_param, 0, sizeof(connect_param));
    memset(&conn_param, 0, sizeof(conn_param));

    client.broker.hostname = MQTT_BROKER_ADDR;
    client.broker.port = MQTT_BROKER_PORT;
    client.evt_cb = mqtt_evt_handler;
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.transport.rx_buf = rx_buffer;
    client.transport.rx_buf_size = sizeof(rx_buffer);
    client.transport.tx_buf = tx_buffer;
    client.transport.tx_buf_size = sizeof(tx_buffer);

    connect_param.client_id.utf8 = "zephyr_client";
    connect_param.client_id.size = strlen("zephyr_client");
    connect_param.clean_session = 1;
    connect_param.keepalive = 60;

    conn_param.connect_timeout_ms = 5000;
    conn_param.keepalive_timeout_ms = 60000;

    rc = mqtt_connect(&client, &connect_param, &conn_param);
    if (rc) {
        printk("Failed to connect to MQTT broker: %d\n", rc);
        return;
    }

    while (1) {
        k_sleep(K_SECONDS(5));
        publish_state(&client);
    }
}

int main(void)
{
    printk("Starting MQTT client\n");
    main_thread();
    return 0;
}
