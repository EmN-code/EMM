
#include <zephyr.h>
#include <net/socket.h>
#include <stdio.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE_MSG "work"
#define SEND_INTERVAL_SECONDS 5

void main(void)
{
    int sock;
    struct sockaddr_in server_addr;
    int ret;

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    // Set up the server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    while (1) {
        // Send the device state message
        ret = sendto(sock, DEVICE_STATE_MSG, strlen(DEVICE_STATE_MSG), 0,
                     (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (ret < 0) {
            printk("Failed to send data\n");
        } else {
            printk("Sent device state: %s\n", DEVICE_STATE_MSG);
        }

        // Wait for 5 seconds before sending the next message
        k_sleep(K_SECONDS(SEND_INTERVAL_SECONDS));
    }

    // Close the socket (this part is unreachable in this example)
    close(sock);
}
