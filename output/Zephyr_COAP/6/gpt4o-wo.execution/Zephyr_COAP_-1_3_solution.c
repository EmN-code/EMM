
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/sys/printk.h>
#include <zephyr/syscalls/kernel.h>
#include <sys/time.h>
#include <zephyr/random/random.h>
#include <zephyr/net/websocket.h>
#include <zephyr/pm/state.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/net/buf.h>
#include <zephyr/devicetree.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/sys/slist.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/sys/util.h>
#include <zephyr/syscalls/device.h>
#include <time.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/posix/poll.h>
#include <zephyr/types.h>
#include <zephyr/syscalls/i2c.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

int main(void) {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[128];

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    // Set up the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Prepare the CoAP packet
    memset(buffer, 0, sizeof(buffer));
    // CoAP header (4 bytes)
    buffer[0] = 0x40; // Version 1, Type Confirmable, Token length 0
    buffer[1] = 0x01; // Code POST
    buffer[2] = 0x00; // Message ID MSB
    buffer[3] = 0x00; // Message ID LSB

    // Option: Uri-Path (0xB0)
    buffer[4] = 0xB0 | (strlen(URI_PATH) & 0x0F); // Option delta 11, length of URI_PATH
    memcpy(buffer + 5, URI_PATH, strlen(URI_PATH));

    // Payload marker (0xFF)
    buffer[5 + strlen(URI_PATH)] = 0xFF;

    // Payload
    memcpy(buffer + 6 + strlen(URI_PATH), MESSAGE, strlen(MESSAGE));

    // Send the CoAP packet
    if (sendto(sock, buffer, 6 + strlen(URI_PATH) + strlen(MESSAGE), 0,
               (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printk("Failed to send CoAP request\n");
        close(sock);
        return -1;
    }

    printk("CoAP request sent successfully\n");

    // Close the socket
    close(sock);

    return 0;
}
