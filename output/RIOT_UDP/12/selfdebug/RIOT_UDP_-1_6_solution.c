#include "xtimer.h"
#include <stdio.h>
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * 1000000U) // 5 seconds in microseconds

int main(void) {
    sock_udp_ep_t remote = {.family = AF_INET, .port = SERVER_PORT};
    sock_udp_t sock;
    struct sockaddr_in server_addr;

    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);
    remote.addr.ipv4 = server_addr.sin_addr;

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error creating UDP socket\n");
        return 1;
    }

    while (1) {
        const char *device_state = "work"; // or "not work"
        if (sock_udp_send(&sock, device_state, strlen(device_state), &remote) < 0) {
            printf("Error sending message\n");
        } else {
            printf("Message sent: %s\n", device_state);
        }

        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}