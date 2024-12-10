
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "sys/socket.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE_LEN 12
#define INTERVAL_SEC 5

void send_device_state(int sockfd, const char *state) {
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_aton(SERVER_IP, &server_addr.sin_addr);

    ssize_t bytes_sent = sendto(sockfd, state, strlen(state), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (bytes_sent < 0) {
        perror("sendto");
    } else {
        printf("Sent device state: %s\n", state);
    }
}

int main(void) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    while (1) {
        send_device_state(sockfd, "work");
        sleep(INTERVAL_SEC);
        send_device_state(sockfd, "not work");
        sleep(INTERVAL_SEC);
    }

    close(sockfd);
    return 0;
}
