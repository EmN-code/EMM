#include "xtimer.h"
#include "net/sock/udp.h"
#include <stdio.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define UDP_BUF_SIZE 64

static sock_udp_ep_t remote;

int main(void) {
    // Initialize the remote endpoint
    remote.port = SERVER_PORT;
    remote.family = AF_INET;
    inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4);

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: unable to create UDP sock\n");
        return -1;
    }

    const char *device_state = "work"; // Device state to send

    while (1) {
        if (sock_udp_send(&sock, device_state, strlen(device_state), &remote) < 0) {
            printf("Error: unable to send data\n");
        } else {
            printf("Device state sent: %s\n", device_state);
        }

        xtimer_sleep(5); // Sleep for 5 seconds before sending again
    }

    sock_udp_close(&sock);
    return 0;
}