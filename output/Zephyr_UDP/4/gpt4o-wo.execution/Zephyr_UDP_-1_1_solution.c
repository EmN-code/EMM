#include <zephyr.h>
#include <sys/socket.h>
#include <net/socket.h>
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001

void udp_send_device_state(void) {
    int sock;
    struct sockaddr_in server_addr;
    char *device_state = "work"; // Example state

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        printf("Failed to create socket\n");
        return;
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    while (1) {
        // Send device state to server
        if (sendto(sock, device_state, strlen(device_state), 0,
                   (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            printf("Failed to send message\n");
        } else {
            printf("Message sent: %s\n", device_state);
        }

        // Wait for 5 seconds
        k_sleep(K_SECONDS(5));
    }

    // Close the socket
    close(sock);
}

int main(void) {
    printf("UDP Client Example\n");
    udp_send_device_state();
    return 0;
}