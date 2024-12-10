#include <zephyr/syscalls/i2c.h>
#include <zephyr/types.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/kernel.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/net/wifi.h>
#include <zephyr/mqtt_sn.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/net/buf.h>
#include <zephyr/sys/slist.h>
#include <zephyr/init.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/devicetree.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/websocket.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/fs/nvs.h>

int main() {
    struct sockaddr_in server_addr;
    int sock;
    uint8_t coap_buffer[128];
    struct coap_packet cpkt;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5683);
    inet_pton(AF_INET, "47.102.103.1", &server_addr.sin_addr);

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printk("Failed to connect socket\n");
        close(sock);
        return -1;
    }

    int r = coap_packet_init(&cpkt, coap_buffer, sizeof(coap_buffer), 1, COAP_TYPE_CON, COAP_TOKEN_MAX_LEN, NULL, COAP_METHOD_POST, coap_next_id());
        if (r < 0) {
        printk("Cannot initialize CoAP packet\n");
        close(sock);
        return -1;
    }

    if (coap_packet_append_uri_path(&cpkt, "coap/test") < 0) {
        printk("Cannot append uri path\n");
        close(sock);
        return -1;
    }

    if (coap_packet_append_payload_marker(&cpkt) < 0) {
        printk("Cannot append payload marker\n");
        close(sock);
        return -1;
    }

    const char payload[] = "Hello, COAP Cloud";
    if (coap_packet_append_payload(&cpkt, (uint8_t *)payload, sizeof(payload) - 1) < 0) {
        printk("Cannot append payload\n");
        close(sock);
        return -1;
    }

    if (send(sock, coap_buffer, cpkt.offset, 0) < 0) {
        printk("Cannot send data\n");
        close(sock);
        return -1;
    }

    printk("CoAP message sent!\n");
    close(sock);
    return 0;
}
