#include <zephyr/net/wifi.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/net/mqtt.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define PUBLISH_INTERVAL 5000 // in milliseconds

void main(void) {
    int sock;
    struct sockaddr_in addr;
    const char* msg = "work or not work";

    // Initialize socket
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printk("Failed to create socket
");
        return;
    }

    // Configure server address
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &addr.sin_addr);

    // Connect to MQTT broker
    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        printk("Failed to connect
");
        close(sock);
        return;
    }

    // Main loop to publish messages every 5 seconds
    while (1) {
        send(sock, msg, strlen(msg), 0);
        printk("Published: %s
", msg);
        k_msleep(PUBLISH_INTERVAL);
    }

    close(sock);
}