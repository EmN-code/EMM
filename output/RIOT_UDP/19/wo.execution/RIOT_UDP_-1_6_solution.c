#include <ztimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

int main(void) {
    // Set up variables
    const char *server_ip = "47.102.103.1";
    const int server_port = 20001;
    const char device_state[] = "work"; // or "not work"

    // Create UDP socket
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    // Define server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        return 1;
    }

    // Send device state every five seconds
    while (1) {
        ssize_t sent = sendto(sock, device_state, strlen(device_state), 0,
                              (struct sockaddr *) &server_addr, sizeof(server_addr));
        if (sent < 0) {
            perror("sendto");
        } else {
            printf("Sent %ld bytes to %s:%d\n", sent, server_ip, server_port);
        }

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    // Close socket (though we will never reach here in this infinite loop)
    close(sock);

    return 0;
}