#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>

#define SERVER_PORT 20001
#define SERVER_IP "47.102.103.1"

void main(void) {
    int sock;
    struct sockaddr_in server_addr;
    const char *state_working = "work";
    const char *state_not_working = "not work";
    int state_toggle = 0;

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Error creating socket\n");
        return;
    }

    // Configure the server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    while (1) {
        // Send UDP packet
        const char *message = state_toggle ? state_working : state_not_working;
        ssize_t sent = sendto(sock, message, strlen(message), 0,
                              (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (sent < 0) {
            printk("Error sending UDP message\n");
        } else {
            printk("Sent: %s\n", message);
        }

        // Toggle state
        state_toggle = !state_toggle;

        // Wait for 5 seconds
        k_sleep(K_SECONDS(5));
    }

    // Close the socket (though this is unreachable in the loop)
    close(sock);
}