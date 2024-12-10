
#include <zephyr/device.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/tracing/tracing_macros.h>
#include <sys/time.h>
#include <zephyr/sys/printk.h>
#include <zephyr/random/random.h>
#include <zephyr/net/websocket.h>
#include <zephyr/kernel.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/buf.h>
#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/socket.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/net/wifi.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/syscalls/device.h>
#include <time.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/device.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/kernel_includes.h>

#include <stdio.h>
#include <string.h>
#include <zephyr/net/socket.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

int main(void) {
    int sock;
    struct sockaddr_in server_addr;
    char *message = "Hello, COAP Cloud";
    char buffer[128];

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    // Set up the server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Send the CoAP POST request
    int len = snprintf(buffer, sizeof(buffer), "POST %s HTTP/1.1\r\nContent-Length: %zu\r\n\r\n%s", URI_PATH, strlen(message), message);
    if (sendto(sock, buffer, len, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printk("Failed to send data\n");
        close(sock);
        return -1;
    }

    printk("CoAP request sent successfully\n");

    // Close the socket
    close(sock);

    return 0;
}
