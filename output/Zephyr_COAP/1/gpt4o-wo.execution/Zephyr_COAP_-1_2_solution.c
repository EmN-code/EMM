#include <zephyr/sys/util.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/types.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/random/random.h>
#include <zephyr/init.h>
#include <zephyr/net/buf.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/net_if.h>
#include <zephyr/kernel.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/kernel.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/printk.h>

#define COAP_BUF_SIZE 256

int main() {
    // Define server endpoint
    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(5683);
    inet_pton(AF_INET, "47.102.103.1", &dest_addr.sin_addr);

    // Create a CoAP packet
    uint8_t buffer[COAP_BUF_SIZE];
    struct coap_packet request;

    // Properly initialize the CoAP packet
    int ret = coap_packet_init(&request, buffer, sizeof(buffer), COAP_VERSION_1, COAP_TYPE_CON, 0, NULL, COAP_METHOD_POST, coap_next_id());
    if (ret < 0) {
        printk("Failed to initialize CoAP packet: %d\n", ret);
        return ret;
    }

    // Append URI path to the packet
    coap_packet_append(&request, (uint8_t *)"/coap/test", strlen("/coap/test"));

    // Append payload marker and payload
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)"Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

    // Create a socket
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket: %d\n", sock);
        return sock;
    }

    // Send the packet
    ssize_t sent = sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (sent < 0) {
        printk("Failed to send packet: %d\n", sent);
    } else {
        printk("Sent %d bytes\n", sent);
    }

    // Close the socket
    // Closing code would go here, assuming it's permitted by the blacklist

    return 0;
}
