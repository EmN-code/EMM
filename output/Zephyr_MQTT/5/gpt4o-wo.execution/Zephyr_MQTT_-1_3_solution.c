#include <zephyr/sys/util.h>
#include <zephyr/sys/slist.h>
#include <zephyr/syscmds/device.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/random/random.h>
#include <zephyr/net/buf.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/stats/stats.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/wifi.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/net/wifi.h>
#include <time.h>
#include <sys/time.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID "zephyr_publisher"
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work or not work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0

static struct mqtt_client client;
static struct sockaddr_in broker;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];

static void prepare_fds(struct mqtt_client *client, struct pollfd *fds) {
    fds->fd = client->transport.tcp.sock;
    fds->events = POLLIN;
    fds->revents = 0;
}

static int try_to_connect(struct sockaddr_in *broker) {
    int rc;
    struct mqtt_sec_config sec_config;

    mqtt_client_init(&client);

    broker->sin_family = AF_INET;
    broker->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker->sin_addr);

    client.broker = (struct sockaddr *)broker;
    client.evt_cb = mqtt_event_handler;
    client.client_id.utf8 = (uint8_t *)CLIENT_ID;
    client.client_id.size = strlen(CLIENT_ID);

    client.protocol_version = MQTT_VERSION_3_1_1;

    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    sec_config = (struct mqtt_sec_config)MQTT_SECURITY_DISABLED;
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    rc = mqtt_connect(&client, &sec_config);

    return rc;
}

void mqtt_event_handler(struct mqtt_client *const client, const struct mqtt_evt *evt) {
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        printk("MQTT client connected!");
        break;

    case MQTT_EVT_DISCONNECT:
        printk("MQTT client disconnected!");
        break;

    case MQTT_EVT_PUBLISH:
        printk("MQTT message published!");
        break;

    default:
        printk("Unhandled MQTT event %d", evt->type);
        break;
    }
}

static void publish_state(void) {
    struct mqtt_publish_param param;

    param.message.topic.topic.utf8 = (uint8_t *)MQTT_TOPIC;
    param.message.topic.topic.size = strlen(MQTT_TOPIC);
    param.message.payload.data = (uint8_t *)MQTT_PAYLOAD;
    param.message.payload.len = strlen(MQTT_PAYLOAD);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = MQTT_RETAIN;
    param.message.topic.qos = MQTT_QOS;

    mqtt_publish(&client, &param);
}

void main(void) {
    struct pollfd fds;
    int rc;

    rc = try_to_connect(&broker);
    if (rc != 0) {
        printk("Failed to connect to broker, error: %d", rc);
        return;
    }

    prepare_fds(&client, &fds);

    while (1) {
        rc = poll(&fds, 1, K_SECONDS(5));
        if (rc > 0 && (fds.revents & POLLIN) != 0) {
            mqtt_input(&client);
        }

        mqtt_live(&client);
        publish_state();
        k_sleep(K_SECONDS(5));
    }
}