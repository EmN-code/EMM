#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>

#define SERVER_PORT 20001
#define SERVER_ADDR "47.102.103.1"
#define SEND_INTERVAL K_SECONDS(5)  // Send every 5 seconds
#define STATE_MESSAGE "work or not work"

void main(void) {
    struct sockaddr_in server_addr;
    int sock;
    int ret;

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create UDP socket\n");
        return;
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    while (1) {
        // Send state message to the server
        ret = sendto(sock, STATE_MESSAGE, sizeof(STATE_MESSAGE),
                     0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (ret < 0) {
            printk("Failed to send message\n");
        } else {
            printk("Message sent: %s\n", STATE_MESSAGE);
        }

        // Wait for the specified interval
        k_sleep(SEND_INTERVAL);
    }

    // Close the socket (This will actually never be reached, but it's good practice)
    close(sock);
}