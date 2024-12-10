#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdatomic.h>
#include <stddef.h>
#include <event/thread.h>
#include <dht_params.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define RETRY_INTERVAL 5

int main(void) {
    int sock;
    struct sockaddr_in server_addr;
    int rc;
    const char *device_state = DEVICE_STATE;

    // Create UDP socket
    while (1) {
        sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (sock < 0) {
            printf("Socket creation failed. Retrying...\n");
            sleep(RETRY_INTERVAL);
            continue;
        }
        else {
            printf("Socket created successfully.\n");
            break;
        }
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    rc = inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);
    if (rc <= 0) {
        printf("Invalid address/ Address not supported.\n");
        close(sock);
        return -1;
    }

    // Send device state every 5 seconds
    while (1) {
        rc = sendto(sock, device_state, strlen(device_state), 0,
                    (const struct sockaddr *)&server_addr, sizeof(server_addr));
        if (rc < 0) {
            printf("Failed to send device state. Retrying...\n");
            close(sock);
            sleep(RETRY_INTERVAL);
            continue;
        }
        else {
            printf("Device state sent successfully.\n");
        }

        sleep(RETRY_INTERVAL);
    }

    // Close socket (unreachable code but good practice)
    close(sock);
    return 0;
}
