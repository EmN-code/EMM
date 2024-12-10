#include <zephyr/sys/iterable_sections.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define SLEEP_TIME_MS 5000

void main(void) {
    struct sockaddr_in server_addr;
    int sock;
    const char *device_state = "work or not work";

    // Clear and configure server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    while (1) {
        // Send device state to server
        int sent_bytes = sendto(sock, device_state, strlen(device_state), 0,
                                (struct sockaddr *)&server_addr, sizeof(server_addr));

        if (sent_bytes < 0) {
            printk("Failed to send message\n");
        } else {
            printk("Message sent: %s\n", device_state);
        }

        // Wait for 5 seconds
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    // Close the socket
    close(sock);
}