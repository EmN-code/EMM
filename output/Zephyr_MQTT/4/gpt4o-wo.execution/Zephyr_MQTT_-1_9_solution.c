#include <zephyr/types.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/kernel.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/device.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/sys/printk.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/random/random.h>
#include <zephyr/net/buf.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/stats/stats.h>
#include <zephyr/pm/state.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/posix/poll.h>
#include <zephyr/sys/slist.h>
#include <zephyr/syscalls/i2c.h>
#include <sys/time.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/linker/sections.h>
#include <zephyr/devicetree.h>
#include <sys/time.h>
#include <zephyr/sys/util.h>
#include <zephyr/device.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/websocket.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/net/tls_credentials.h>
#include <zephyr/fs/nvs.h>

static void publish_state(struct mqtt_client *client) {
    const struct mqtt_publish_param param = {
        .message.topic = {.topic = {"work_status", sizeof("work_status") - 1}},
        .message.payload.data = "work",
        .message.payload.len = sizeof("work") - 1,
        .message.qos = MQTT_QOS_0,
        .retain_flag = 0,
    };
    mqtt_publish(client, &param);
}

static void init_mqtt_client(struct mqtt_client *client) {
    mqtt_client_init(client);
    client->broker = {
        .addr = "47.102.103.1",
        .port = 1883,
    };
    client->evt_cb = NULL;
    client->client_id.utf8 = "zephyr_pub_client";
    client->client_id.size = strlen("zephyr_pub_client");
}

int main() {
    struct mqtt_client client;
    init_mqtt_client(&client);
    if (mqtt_connect(&client) != 0) {
        printk("Could not connect to MQTT broker\n");
        return -1;
    }
    while (1) {
        publish_state(&client);
        k_sleep(K_SECONDS(5));
    }
    mqtt_disconnect(&client);
    return 0;
}