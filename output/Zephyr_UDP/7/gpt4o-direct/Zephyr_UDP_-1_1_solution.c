
#include <zephyr.h>
#include <net/socket.h>
#include <stdio.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE_WORK "work"
#define DEVICE_STATE_NOT_WORK "not work"
#define SEND_INTERVAL_SECONDS 5

void send_udp_message(int sock, const char *message) {
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    sendto(sock, message, strlen(message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
}

int main(void) {
    int sock;
    struct sockaddr_in local_addr;

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("Failed to create socket\n");
        return -1;
    }

    // Bind socket to local address
    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    local_addr.sin_port = htons(0);

    if (bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
        printf("Failed to bind socket\n");
        close(sock);
        return -1;
    }

    // Main loop to send device state every 5 seconds
    while (1) {
        // Simulate device state (work or not work)
        const char *device_state = (rand() % 2 == 0) ? DEVICE_STATE_WORK : DEVICE_STATE_NOT_WORK;

        // Send device state to UDP server
        send_udp_message(sock, device_state);
        printf("Sent device state: %s\n", device_state);

        // Wait for 5 seconds before sending the next state
        k_sleep(K_SECONDS(SEND_INTERVAL_SECONDS));
    }

    // Close socket (this part will never be reached in this example)
    close(sock);

    return 0;
}
