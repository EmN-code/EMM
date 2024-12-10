#include <zephyr/net/socket.h>
#include <zephyr/sys/util.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/net/buf.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/stats/stats.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>
#include <zephyr/random/random.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/sys/device_mmio.h>
#include <time.h>
#include <sys/time.h>

#define MQTT_CLIENTID    "zephyr_client"
#define MQTT_BROKER_IP   "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC       "device/state"
#define PUBLISH_INTERVAL K_SECONDS(5)
#define MQTT_PAYLOAD     "work or not work"

static struct mqtt_client client;
static struct sockaddr_storage broker;
static bool connected = false;

void mqtt_evt_handler(struct mqtt_client *const c, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printk("MQTT client connected!\n");
            connected = true;
            break;
        case MQTT_EVT_DISCONNECT:
            printk("MQTT client disconnected!\n");
            connected = false;
            break;
        default:
            break;
    }
}

static void prepare_mqtt_client(void) {
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker4->sin_addr);

    mqtt_client_init(&client);

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = MQTT_CLIENTID;
    client.client_id.size = strlen(MQTT_CLIENTID);
    
    client.protocol_version = MQTT_VERSION_3_1_1;
}

void publish_state(void) {
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = MQTT_TOPIC;
    param.message.topic.topic.size = strlen(MQTT_TOPIC);
    param.message.payload.data = MQTT_PAYLOAD;
    param.message.payload.len = strlen(MQTT_PAYLOAD);
    param.retain_flag = 0;
    param.dup_flag = 0;
    
    int rc = mqtt_publish(&client, &param);
    if (rc) {
        printk("Failed to publish message: %d\n", rc);
    }
}

void main(void) {
    printk("Starting MQTT sample\n");
    prepare_mqtt_client();

    int rc = mqtt_connect(&client);
    if (rc != 0) {
        printk("MQTT connect failed: %d\n", rc);
        return;
    }

    while (!connected) {
        mqtt_input(&client);
        mqtt_live(&client);
        k_sleep(K_MSEC(100));
    }

    while (1) {
        publish_state();
        k_sleep(PUBLISH_INTERVAL);
    }
}