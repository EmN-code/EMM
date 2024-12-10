#include "net/emcute.h"
#include "netinet/in.h"
#include "dht_params.h"
#include "esp_system.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "mutex.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include <string.h>
#include <time.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "work"

int main(void) {
    int sockfd;
    struct sockaddr_in server_addr;
    
    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    // Setting up server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_aton(SERVER_IP, &server_addr.sin_addr);

    char message[] = MESSAGE;

    while (1) {
        // Send message to server
        ssize_t sent_bytes = sendto(sockfd, message, strlen(message), 0,
                                    (struct sockaddr*)&server_addr,
                                    sizeof(server_addr));
        if (sent_bytes < 0) {
            perror("sendto");
        }

        // Sleep for 5 seconds (5,000,000,000 nanoseconds)
        struct timespec ts = {5, 0};
        nanosleep(&ts, NULL);
    }

    // Close the socket
    close(sockfd);
    return 0;
}