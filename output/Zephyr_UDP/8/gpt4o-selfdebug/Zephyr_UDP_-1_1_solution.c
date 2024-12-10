#include <zephyr/types.h>
#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>

#define SERVER_PORT 20001
#define SERVER_ADDR "47.102.103.1"
#define SEND_INTERVAL K_SECONDS(5)

void udp_send_state(const char *server_addr, uint16_t server_port, const char *message) {
    struct sockaddr_in dest_addr;
    int sock;

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create UDP socket\n");
        return;
    }

    // Set up destination address
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_addr, &dest_addr.sin_addr);

    while (1) {
        // Send message to the server
        int ret = sendto(sock, message, strlen(message), 0,
                         (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        if (ret < 0) {
            printk("Failed to send UDP message\n");
        } else {
            printk("UDP message sent: %s\n", message);
        }

        // Wait for the next send interval
        k_sleep(SEND_INTERVAL);
    }

    // Cleanup, close the socket
    close(sock);
}

int main(void) {
    const char *device_state = "work"; // For demo purposes, this is hardcoded

    // Print a message indicating that the device is ready
    printk("Device ready to send UDP messages\n");

    // Start sending the device state every five seconds
    udp_send_state(SERVER_ADDR, SERVER_PORT, device_state);

    return 0;
}