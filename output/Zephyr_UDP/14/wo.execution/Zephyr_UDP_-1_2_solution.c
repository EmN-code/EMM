
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/net/socket.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define SEND_INTERVAL 5000  // 5 seconds

int main(void) {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[256];

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    // Set up the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        printk("Invalid address/ Address not supported\n");
        return -1;
    }

    // Main loop to send device state every 5 seconds
    while (1) {
        // Prepare the message
        snprintf(buffer, sizeof(buffer), "Device state: %s", DEVICE_STATE);

        // Send the message to the server
        if (sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            printk("Failed to send message\n");
        } else {
            printk("Message sent: %s\n", buffer);
        }

        // Wait for 5 seconds before sending the next message
        k_sleep(K_MSEC(SEND_INTERVAL));
    }

    // Close the socket (this part is unreachable in this example)
    close(sock);

    return 0;
}
