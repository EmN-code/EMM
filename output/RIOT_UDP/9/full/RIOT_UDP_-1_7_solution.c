#include "net/sock/util.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "stdio.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "msg.h"

int main() {
    // Define server address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(20001);
    inet_pton(AF_INET, "47.102.103.1", &server_addr.sin_addr);

    // Create UDP socket
    int sock;
    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Send device state to server every 5 seconds
    char *device_state = "device: work or not work";
    while (1) {
        ssize_t sent_size = sendto(sock, device_state, strlen(device_state), 0, 
                                   (struct sockaddr *)&server_addr, sizeof(server_addr));

        if (sent_size < 0) {
            perror("sendto failed");
        } else {
            printf("Sent: %s\n", device_state);
        }

        // Sleep for 5 seconds
        sleep(5);
    }

    // Close the socket
    close(sock);
    return 0;
}