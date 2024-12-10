
#include <zephyr/sys/device_mmio.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/sys/printk.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/pm/state.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/slist.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/wifi.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/net/websocket.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/net/buf.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/device.h>
#include <sys/time.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/posix/poll.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/random/random.h>
#include <zephyr/devicetree.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/sys/util.h>
#include <time.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/types.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

int main(void) {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[128];
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
    memset(buffer, 0, sizeof(buffer));
    uint8_t *ptr = (uint8_t *)buffer;

    // CoAP version, type, token length, and code
    *ptr++ = (1 << 6) | (0 << 4) | 0; // CoAP version 1, Confirmable, Token length 0
    *ptr++ = 0x02; // POST method

    // Message ID
    *ptr++ = 0x00;
    *ptr++ = 0x01;

    // URI-Path option
    *ptr++ = 0xB1; // Option delta 11, length 1
    *ptr++ = '/';
    *ptr++ = 0xB1; // Option delta 11, length 1
    *ptr++ = 'c';
    *ptr++ = 0xB1; // Option delta 11, length 1
    *ptr++ = 'o';
    *ptr++ = 0xB1; // Option delta 11, length 1
    *ptr++ = 'a';
    *ptr++ = 0xB1; // Option delta 11, length 1
    *ptr++ = 'p';
    *ptr++ = 0xB1; // Option delta 11, length 1
    *ptr++ = '/';
    *ptr++ = 0xB1; // Option delta 11, length 1
    *ptr++ = 't';
    *ptr++ = 0xB1; // Option delta 11, length 1
    *ptr++ = 'e';
    *ptr++ = 0xB1; // Option delta 11, length 1
    *ptr++ = 's';
    *ptr++ = 0xB1; // Option delta 11, length 1
    *ptr++ = 't';

    // Payload marker
    *ptr++ = 0xFF;

    // Payload
    memcpy(ptr, PAYLOAD, strlen(PAYLOAD));
    ptr += strlen(PAYLOAD);

    // Send the CoAP packet
    ret = sendto(sock, buffer, ptr - (uint8_t *)buffer, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        printk("Failed to send CoAP request\n");
        close(sock);
        return -1;
    }

    printk("CoAP request sent successfully\n");

    // Close the socket
    close(sock);

    return 0;
}
