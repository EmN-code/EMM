#include <stdlib.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include "kernel_defines.h"
#include "net/sock/udp.h"
#include "ztimer.h"

#define BUF_SIZE 128
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001

void udp_send_state(int sock, struct sockaddr_in *server_addr) {
    char buffer[BUF_SIZE];
    snprintf(buffer, BUF_SIZE, "device state: %s", "work");

    while (sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)server_addr, sizeof(*server_addr)) < 0) {
        printf("sendto() failed, retrying in 1 second...\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    printf("Device state sent successfully: %s\n", buffer);
}

int main(void) {
    int sock;
    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        perror("socket creation failed");
        return EXIT_FAILURE;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("inet_pton() failed");
        close(sock);
        return EXIT_FAILURE;
    }

    while (1) {
        udp_send_state(sock, &server_addr);
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    close(sock);
    return 0;
}