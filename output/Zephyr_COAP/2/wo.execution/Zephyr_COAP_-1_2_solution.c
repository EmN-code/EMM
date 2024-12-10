
#include <zephyr/kernel.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/slist.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/posix/poll.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/random/random.h>
#include <zephyr/device.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/util.h>
#include <zephyr/types.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/buf.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/sys/device_mmio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

int main(void) {
    int sock;
    struct sockaddr_in server_addr;
    struct pollfd fds;
    int ret;

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    // Set up the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Prepare the CoAP packet
    uint8_t coap_packet[128];
    uint8_t *ptr = coap_packet;

    // CoAP header
    *ptr++ = 0x40; // Version 1, Type 0 (CON), Token length 0
    *ptr++ = 0x01; // Code 0.01 (POST)
    *ptr++ = 0x00; // Message ID (MSB)
    *ptr++ = 0x00; // Message ID (LSB)

    // URI-Path option
    *ptr++ = 0xB1; // Option Delta 11, Length 1
    *ptr++ = '/';
    *ptr++ = 0xB4; // Option Delta 11, Length 4
    memcpy(ptr, "coap", 4);
    ptr += 4;
    *ptr++ = 0xB4; // Option Delta 11, Length 4
    memcpy(ptr, "test", 4);
    ptr += 4;

    // Payload marker
    *ptr++ = 0xFF;

    // Payload
    memcpy(ptr, MESSAGE, strlen(MESSAGE));
    ptr += strlen(MESSAGE);

    // Send the CoAP packet
    ret = sendto(sock, coap_packet, ptr - coap_packet, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        printk("Failed to send CoAP packet\n");
        close(sock);
        return -1;
    }

    printk("CoAP packet sent successfully\n");

    // Close the socket
    close(sock);

    return 0;
}
