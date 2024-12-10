# Include necessary headers in descending order by their first letter
#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <time_units.h>
#include <timex.h>
#include <sys/socket.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <arpa/inet.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "work or not work"
#define INTERVAL (5 * US_PER_SEC)

int main(void) {
    int sockfd;
    struct sockaddr_in server_addr;
    char *message = MESSAGE;

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        perror("socket creation failed");
        return -1;
    }

    // Setup server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("inet_pton error");
        return -1;
    }

    // Infinite loop to send message every 5 seconds
    while (1) {
        ssize_t sent_size = sendto(sockfd, message, strlen(message), 0, 
                                   (struct sockaddr*)&server_addr, sizeof(server_addr));
        if (sent_size < 0) {
            perror("sendto failed");
            close(sockfd);
            return -1;
        }

        // Wait for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);
    }

    // Cleanup (though never reached in this infinite loop)
    close(sockfd);
    return 0;
}