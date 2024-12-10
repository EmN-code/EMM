#include <zephyr/syscalls/random.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/stats/stats.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/slist.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/types.h>
#include <zephyr/time.h>
#include <zephyr/stats.h>
#include <zephyr/syscalls.h>
#include <zephyr/linker/sections.h>
#include <zephyr/kernel_includes.h>

// Define MQTT client
static struct mqtt_client client;

// Function to publish messages
void publish_message() {
    struct mqtt_publish_param param;
    struct mqtt_publish_message msg;

    msg.topic = "esp32/status";
    msg.payload.data = "work";
    msg.payload.len = strlen("work");

    param.message = msg;
    param.message_id = 1;
    param.retain_flag = 0;

    mqtt_publish(&client, &param);
}

// Main function
int main() {
    struct sockaddr_in broker;
    int rc;

    broker.sin_family = AF_INET;
    broker.sin_port = htons(1883);
    inet_pton(AF_INET, "47.102.103.1", &broker.sin_addr);

    mqtt_client_init(&client);
    client.broker = &broker;

    rc = mqtt_connect(&client);
    if (rc != 0) {
        printk("Could not connect, error %d", rc);
        return rc;
    }

    while (1) {
        publish_message();
        sleep(5);
    }

    mqtt_disconnect(&client);
    return 0;
}