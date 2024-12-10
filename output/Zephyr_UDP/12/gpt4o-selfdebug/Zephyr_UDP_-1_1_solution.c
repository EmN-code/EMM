#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001

void main(void) {
    struct sockaddr_in server_addr;
    int sock;
    const char *device_state = "work";

    // Initialize server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create UDP socket\n");
        return;
    }

    while (1) {
        // Send device state to the server
        sendto(sock, device_state, strlen(device_state), 0,
               (struct sockaddr *)&server_addr, sizeof(server_addr));

        printk("Device state sent: %s\n", device_state);

        // Sleep for 5 seconds
        k_sleep(K_SECONDS(5));
    }

    // Close the socket
    close(sock);
}