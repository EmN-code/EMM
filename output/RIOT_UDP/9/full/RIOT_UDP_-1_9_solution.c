#include "ztimer.h"
#include "stdio.h"
#include "stdlib.h"
#include "net/sock/udp.h"
#include "net/af.h"
#include "arpa/inet.h"
#include "stdbool.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * 1000000U)  // 5 seconds in microseconds

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };

    // Convert IP address from string to network format
    if (inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4) != 1) {
        printf("Error: Unable to parse IP address\n");
        return 1;
    }

    // Create UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: Unable to create UDP socket\n");
        return 1;
    }

    char *state_msg;
    while (1) {
        state_msg = "work";  // Assume device is in "work" state

        if (sock_udp_send(&sock, state_msg, strlen(state_msg), &server) < 0) {
            printf("Error: Unable to send UDP packet\n");
        }
        else {
            printf("Sent state: %s\n", state_msg);
        }

        // Wait for the next interval
        ztimer_sleep(ZTIMER_USEC, SEND_INTERVAL);
    }

    // Close UDP socket (not reached due to infinite loop)
    sock_udp_close(&sock);

    return 0;
}