#include <ztimer.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <net/sock/udp.h>
#include <net/inet/pton.h> 

#define UDP_SERVER_PORT 20001
#define UDP_SERVER_ADDR "47.102.103.1"
#define INTERVAL (5U * 1000000U)  // 5 seconds in microseconds

static sock_udp_ep_t remote;

int main(void) {
    ztimer_sleep(ZTIMER_USEC, 1000000); // Initial delay, if needed

    // Initialize the remote endpoint (server) address
    remote.family = AF_INET;
    remote.port = UDP_SERVER_PORT;
    inet_pton(AF_INET, UDP_SERVER_ADDR, &remote.addr.ipv4);

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    while (1) {
        // Prepare the message
        char message[] = "work";

        // Send the message
        ssize_t res = sock_udp_send(&sock, message, strlen(message), &remote);
        if (res <= 0) {
            printf("Error sending message: %d\n", (int)res);
        } else {
            printf("Sent message: %s\n", message);
        }

        // Wait for the next interval
        ztimer_sleep(ZTIMER_USEC, INTERVAL);
    }

    return 0;
}