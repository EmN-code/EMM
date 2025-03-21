
#include <zephyr.h>
#include <net/socket.h>
#include <stdio.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define SEND_INTERVAL 5000  // 5 seconds

void send_udp_message(int sock, const char *message) {
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    if (sendto(sock, message, strlen(message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Failed to send UDP message\n");
    } else {
        printf("Sent: %s\n", message);
    }
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
    local_addr.sin_port = htons(0);  // Use any available port

    if (bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
        printf("Failed to bind socket\n");
        close(sock);
        return -1;
    }

    while (1) {
        send_udp_message(sock, DEVICE_STATE);
        k_sleep(K_MSEC(SEND_INTERVAL));
    }

    close(sock);
    return 0;
}
