#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/slist.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/stats/stats.h>
#include <zephyr/random/random.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/buf.h>
#include <zephyr/kernel.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/device.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/wifi_mgmt.h>
#include <sys/time.h>
#include <time.h>

int main() {
    struct mqtt_client client;
    struct mqtt_publish_param param;

    mqtt_client_init(&client);
    struct sockaddr_in broker;
    broker.sin_family = AF_INET;
    broker.sin_port = htons(1883);
    inet_pton(AF_INET, "47.102.103.1", &broker.sin_addr);
    
    if(mqtt_connect(&client)) {
        // Handle error
        return -1;
    }

    param.message_id = 1234;  // Example message ID
    param.dup_flag = 0;
    param.retain_flag = 0;

    while (1) {
        param.message.payload.data = "work";
        param.message.payload.len = strlen("work");
        if (mqtt_publish(&client, &param)) {
            // Handle publish error
        }

        param.message.payload.data = "not work";
        param.message.payload.len = strlen("not work");
        if (mqtt_publish(&client, &param)) {
            // Handle publish error
        }

        sleep(5);  // 5 seconds delay
    }

    mqtt_disconnect(&client);
    return 0;
}