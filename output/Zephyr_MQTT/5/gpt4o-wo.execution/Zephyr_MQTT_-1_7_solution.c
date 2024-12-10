#include <zephyr/kernel.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/random/random.h>
#include <zephyr/net/buf.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/stats/stats.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/slist.h>
#include <zephyr/syscalls/i2c.h>
#include <time.h>
#include <sys/time.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/sys/util.h>
#include <zephyr/device.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/websocket.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/fs/nvs.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD_WORK "work"
#define MQTT_PAYLOAD_NOT_WORK "not work"
#define MQTT_PUBLISH_INTERVAL K_SECONDS(5)

void main(void) {
    printk("Starting MQTT client example for ESP32\n");

    // Initialize MQTT client structure
    struct mqtt_client client;
    mqtt_client_init(&client);

    // Connect to the MQTT broker
    int rc = mqtt_connect(&client);
    if (rc != 0) {
        printk("Failed to connect to MQTT broker: %d\n", rc);
        return;
    }

    while (1) {
        // Prepare and send MQTT publish message
        struct mqtt_publish_param publish_param;
        memset(&publish_param, 0, sizeof(publish_param));
        publish_param.message.topic = MQTT_TOPIC;
        publish_param.message.payload.data = MQTT_PAYLOAD_WORK;
        publish_param.message.payload.len = strlen(MQTT_PAYLOAD_WORK);

        rc = mqtt_publish(&client, &publish_param);
        if (rc != 0) {
            printk("Failed to publish message: %d\n", rc);
        }

        k_sleep(MQTT_PUBLISH_INTERVAL);
    }
}