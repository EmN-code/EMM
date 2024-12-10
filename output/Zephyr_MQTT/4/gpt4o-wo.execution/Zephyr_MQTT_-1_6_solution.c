#include <zephyr/syscalls/socket.h>
#include <zephyr/posix/poll.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/device.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/random/random.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/slist.h>
#include <zephyr/kernel.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/sys/printk.h>
#include <zephyr/stats/stats.h>
#include <zephyr/linker/sections.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/net/net_if.h>
#include <zephyr/device.h>

#include <net/mqtt.h>
#include <net/net_ip.h>

#define MQTT_CLIENTID     "esp32_mqtt_client"
#define SERVER_ADDR       "47.102.103.1"
#define SERVER_PORT       1883
#define TOPIC             "esp32/state"
#define PAYLOAD           "work or not work"
#define KEEP_ALIVE_INTERVAL 60

static struct mqtt_client client;
static struct sockaddr_in broker;
static uint8_t mqtt_rx_buffer[128];
static uint8_t mqtt_tx_buffer[128];
static uint8_t payload_buf[128];

void mqtt_event_handler(struct mqtt_client *const client, const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printk("MQTT client connected!");
            break;
        case MQTT_EVT_DISCONNECT:
            printk("MQTT client disconnected!");
            break;
        case MQTT_EVT_PUBACK:
            printk("PUBACK received for message ID: %u\n", evt->param.puback.message_id);
            break;
        default:
            break;
    }
}

static int publish_message(struct mqtt_client *client)
{
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = TOPIC;
    param.message.topic.topic.size = strlen(TOPIC);
    param.message.payload.data = PAYLOAD;
    param.message.payload.len = strlen(PAYLOAD);

    param.message_id = sys_rand32_get();
    param.dup_flag = 0U;
    param.retain_flag = 0U;

    return mqtt_publish(client, &param);
}

void main(void)
{
    int rc;

    mqtt_client_init(&client);

    broker.sin_family = AF_INET;
    broker.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &broker.sin_addr);

    client.broker = &broker;
    client.evt_cb = mqtt_event_handler;
    client.client_id.utf8 = MQTT_CLIENTID;
    client.client_id.size = strlen(MQTT_CLIENTID);
    client.password = NULL;
    client.user_name = NULL;
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.rx_buf = mqtt_rx_buffer;
    client.rx_buf_size = sizeof(mqtt_rx_buffer);
    client.tx_buf = mqtt_tx_buffer;
    client.tx_buf_size = sizeof(mqtt_tx_buffer);
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;

    rc = mqtt_connect(&client);
    if (rc != 0) {
        printk("Failed to connect to broker, error: %d\n", rc);
        return;
    }

    while (1) {
        rc = mqtt_input(&client);
        if (rc != 0) {
            printk("MQTT input error: %d\n", rc);
            break;
        }
        rc = mqtt_live(&client);
        if (rc != 0) {
            printk("MQTT live error: %d\n", rc);
            break;
        }

        rc = publish_message(&client);
        if (rc != 0) {
            printk("Failed to publish message, error: %d\n", rc);
        }

        k_sleep(K_SECONDS(5));
    }

    mqtt_disconnect(&client);
}