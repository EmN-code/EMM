#define ZM_MACRO_NAME 1

#include <zephyr/syscalls/device.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/net/wifi.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/net/buf.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/stats/stats.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/slist.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/sys/util.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/websocket.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/tracing/tracing_macros.h>

#include <stdbool.h>
#include <zephyr.h>
#include <device.h>
#include <drivers/flash.h>
#include <net/mqtt.h>
#include <random/rand32.h>

#define BROKER_HOSTNAME   "47.102.103.1"
#define BROKER_PORT       1883
#define STACK_SIZE        4096
#define THREAD_PRIORITY   7
#define MQTT_BUFFER_SIZE  256
#define CLIENT_ID         "zephyr_pub"
#define TOPIC "state"

static struct mqtt_publish_param pub_param;
static struct mqtt_client client;
static uint8_t rx_buffer[MQTT_BUFFER_SIZE];
static uint8_t tx_buffer[MQTT_BUFFER_SIZE];
static uint8_t payload[128];

void publish_state(struct mqtt_client *client, char *state) {

    pub_param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    pub_param.message.topic.topic.utf8 = (uint8_t *)TOPIC;
    pub_param.message.topic.topic.size = strlen(TOPIC);
    pub_param.message.payload.data = state;
    pub_param.message.payload.len = strlen(state);
    pub_param.message_id = sys_rand32_get();

    int rc = mqtt_publish(client, &pub_param);
    if (rc) {
        printk("Failed to publish message, error: %d\n", rc);
    } else {
        printk("Published: %s\n", state);
    }
}

void mqtt_evt_handler(struct mqtt_client *client, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printk("MQTT client connected\n");
            break;

        case MQTT_EVT_DISCONNECT:
            printk("MQTT client disconnected\n");
            break;

        default:
            break;
    }
}

void mqtt_setup(struct mqtt_client *client) {
    mqtt_client_init(client);

    client->broker = (struct sockaddr *)&broker;
    client->evt_cb = mqtt_evt_handler;
    client->client_id.utf8 = (uint8_t *)CLIENT_ID;
    client->client_id.size = strlen(CLIENT_ID);
    client->password = NULL;
    client->user_name = NULL;
    client->protocol_version = MQTT_VERSION_3_1_1;

    client->rx_buf = rx_buffer;
    client->rx_buf_size = sizeof(rx_buffer);
    client->tx_buf = tx_buffer;
    client->tx_buf_size = sizeof(tx_buffer);

    client->transport.type = MQTT_TRANSPORT_NON_SECURE;

    mqtt_connect(client);
}

void main(void) {
    struct sockaddr_in broker;

    broker.sin_family = AF_INET;
    broker.sin_port = htons(BROKER_PORT);
    net_addr_pton(AF_INET, BROKER_HOSTNAME, &broker.sin_addr);

    mqtt_setup(&client);

    while (true) {
        publish_state(&client, "work");
        k_sleep(K_SECONDS(5));
        publish_state(&client, "not work");
        k_sleep(K_SECONDS(5));
    }
}