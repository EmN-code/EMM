
#include <zephyr/kernel.h>
#include <time.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/sys/util.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/websocket.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/devicetree.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/net/wifi.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/net/buf.h>
#include <zephyr/sys/slist.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/device.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/net/net_if.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/stats/stats.h>
#include <zephyr/random/random.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/device.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/posix/poll.h>
#include <sys/time.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/posix/netdb.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"

void mqtt_publish_state(int sock) {
    struct sockaddr_in broker_addr;
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.sin_addr);

    char message[50];
    snprintk(message, sizeof(message), "{\"state\":\"%s\"}", MQTT_MESSAGE);

    sendto(sock, message, strlen(message), 0, (struct sockaddr *)&broker_addr, sizeof(broker_addr));
}

int main(void) {
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    while (1) {
        mqtt_publish_state(sock);
        k_sleep(K_SECONDS(5));
    }

    close(sock);
    return 0;
}
