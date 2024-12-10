
#include <zephyr/sys/device_mmio.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/sys/printk.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/pm/state.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/slist.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/net_if.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/device.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/devicetree.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_KEEPALIVE_SEC 60
#define MQTT_QOS 0

static int mqtt_connect(int sock) {
    struct sockaddr_in broker;
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    return connect(sock, (struct sockaddr *)&broker, sizeof(broker));
}

static int mqtt_publish(int sock, const char *topic, const char *payload) {
    uint8_t packet[128];
    uint8_t *ptr = packet;
    uint16_t len = 0;

    // Fixed header
    *ptr++ = 0x30; // PUBLISH (QoS 0)

    // Variable header
    len = strlen(topic);
    *ptr++ = len >> 8;
    *ptr++ = len & 0xFF;
    memcpy(ptr, topic, len);
    ptr += len;

    // Payload
    len = strlen(payload);
    memcpy(ptr, payload, len);
    ptr += len;

    // Remaining length
    *packet = (ptr - packet - 1);

    return send(sock, packet, ptr - packet, 0);
}

int main(void) {
    int sock;

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    if (mqtt_connect(sock) < 0) {
        printk("Failed to connect to MQTT broker\n");
        close(sock);
        return -1;
    }

    while (1) {
        if (mqtt_publish(sock, MQTT_TOPIC, MQTT_PAYLOAD) < 0) {
            printk("Failed to publish message\n");
        } else {
            printk("Message published\n");
        }

        k_sleep(K_SECONDS(5));
    }

    close(sock);
    return 0;
}
