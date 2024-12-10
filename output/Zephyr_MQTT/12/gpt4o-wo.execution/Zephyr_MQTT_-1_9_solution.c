#include <zephyr/net/socket_select.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/wifi.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/sys/kernel.h>
#include <zephyr/net/tls_credentials.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/coap.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/kernel.h>
#include <zephyr/random/random.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <zephyr/types.h>
#include <errno.h>
#include <zephyr/devicetree.h>
#include <zephyr/storage/flash_map.h>
#include <string.h>

#define MQTT_BROKER_ADDRESS "47.102.103.1"
#define MQTT_BROKER_PORT 1883

#define MQTT_CLIENTID "zephyr_client"
#define MQTT_TOPIC "work/state"

#define APP_SLEEP_MSECS 5000

static uint8_t tx_buffer[128];
static uint8_t rx_buffer[128];

/* Buffer for MQTT payload. */
static uint8_t payload_buf[128];

struct mqtt_client client;
struct sockaddr_storage broker;

static void prepare_fds(void) {
    struct pollfd fds = {0};
    fds.fd = client.transport.tcp.sock;
    fds.events = POLLIN;
}

static void connect_to_broker(void) {
    int rc;

    /* Initialize the client structure. */
    mqtt_client_init(&client);

    /* MQTT client configuration */
    client.broker = &broker;
    client.evt_cb = evt_cb;
    client.client_id.utf8 = (uint8_t *)MQTT_CLIENTID;
    client.client_id.size = strlen(MQTT_CLIENTID);
    client.protocol_version = MQTT_VERSION_3_1_1;

    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    client.transport.type = MQTT_TRANSPORT_NON_SECURE;

    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDRESS, &broker4->sin_addr);

    rc = mqtt_connect(&client);
    if (rc != 0) {
        printk("Could not connect to MQTT broker, error: %d\n", rc);
        return;
    }
}

static void evt_cb(struct mqtt_client *const c, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_PUBLISH:
            printk("MQTT PUBLISH event\n");
            break;

        case MQTT_EVT_CONNACK:
            printk("MQTT connection established\n");
            break;

        case MQTT_EVT_DISCONNECT:
            printk("MQTT client disconnected\n");
            break;

        default:
            printk("Unhandled MQTT event: %d\n", evt->type);
            break;
    }
}

void main(void) {
    connect_to_broker();

    while (true) {
        int rc;

        /* Simulate work state by toggling between "work" and "not work" states. */
        static bool work_state = true;
        const char *state_str = work_state ? "work" : "not work";
        work_state = !work_state;

        struct mqtt_publish_param param;
        struct mqtt_publish_message msg;

        msg.topic.topic.utf8 = (uint8_t *)MQTT_TOPIC;
        msg.topic.topic.size = strlen(MQTT_TOPIC);
        msg.payload.data = (uint8_t *)state_str;
        msg.payload.len = strlen(state_str);

        msg.qos = MQTT_QOS_0_AT_MOST_ONCE;

        param.message = msg;
        param.message_id = sys_rand32_get();

        rc = mqtt_publish(&client, &param);
        if (rc != 0) {
            printk("Failed to publish message, error: %d\n", rc);
        }

        k_sleep(K_MSEC(APP_SLEEP_MSECS));
    }
}