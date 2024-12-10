#include <net/sock/udp.h>
#include <net/af.h>
#include <stdio.h>
#include <string.h>
#include <xtimer.h>

#define SERVER_ADDRESS "47.102.103.1"
#define SERVER_PORT (20001)
#define DELAY (5U * US_PER_SEC)

static sock_udp_ep_t server;
static const char *states[] = {"work", "not work"};

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();
    sock_udp_t sock;
    char buffer[64];
    int state_index = 0;

    // Initialize the server endpoint
    server.family = AF_INET;
    server.netif = SOCK_ADDR_ANY_NETIF;
    server.port = SERVER_PORT;

    if (inet_pton(AF_INET, SERVER_ADDRESS, &server.addr.ipv4) != 1) {
        printf("Error: Cannot parse server address\n");
        return 1;
    }

    // Create the UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: Cannot create UDP sock\n");
        return 1;
    }

    while (1) {
        snprintf(buffer, sizeof(buffer), "Device state: %s", states[state_index]);
        
        if (sock_udp_send(&sock, buffer, strlen(buffer), &server) < 0) {
            printf("Error: Cannot send message\n");
        } else {
            printf("Sent: %s\n", buffer);
        }

        // Toggle state for each 5-second interval
        state_index = (state_index + 1) % 2;

        xtimer_periodic_wakeup(&last_wakeup, DELAY);
    }

    return 0;
}