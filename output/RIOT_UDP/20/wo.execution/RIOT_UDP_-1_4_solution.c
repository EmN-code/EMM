#include "event/thread.h"
#include "thread.h"
#include "esp_system.h"
#include "netinet/in.h"
#include "net/emcute.h"
#include "net/af.h"

#define TARGET_IP "47.102.103.1"
#define TARGET_PORT 20001
#define BUF_SIZE 64

int main(void) {
    char buffer[BUF_SIZE];
    struct sockaddr_in server_addr;
    int sockfd;

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        printf("Failed to create socket\n");
        return -1;
    }

    // Setup server address struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(TARGET_PORT);
    inet_pton(AF_INET, TARGET_IP, &server_addr.sin_addr);

    while (1) {
        const char *state = "work";
        snprintf(buffer, BUF_SIZE, "Device state: %s", state);

        // Send device state to server
        int bytes_sent = sendto(sockfd, buffer, strlen(buffer), 0,
                (struct sockaddr *)&server_addr, sizeof(server_addr));

        if (bytes_sent < 0) {
            printf("Failed to send data\n");
        } else {
            printf("Sent %d bytes to %s:%d\n", bytes_sent, TARGET_IP, TARGET_PORT);
        }

        // Wait for 5 seconds
        for (int i = 0; i < 10; i++) {
            thread_yield();
            xtimer_usleep(500000); // 0.5 seconds
        }
    }

    return 0;
}