#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * 1000000U) // 5 seconds in microseconds

int main(void) {
    printf("Starting UDP client example\n");

    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET, .netif = SOCK_ADDR_ANY_NETIF, .port = SERVER_PORT };

    // Parse server IP address
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: could not parse destination address\n");
        return 1;
    }

    // Create UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: unable to create socket\n");
        return 1;
    }

    const char *device_state_work = "work";
    const char *device_state_not_work = "not work";
    char *current_state = (char *) device_state_work; // Assume it starts with work state

    while (1) {
        ssize_t res = sock_udp_send(&sock, current_state, strlen(current_state), &remote);

        if (res < 0) {
            printf("Error sending message\n");
        } else {
            printf("Sent: %s\n", current_state);
            // Toggle state for demonstration
            if (strcmp(current_state, device_state_work) == 0) {
                current_state = (char *) device_state_not_work;
            } else {
                current_state = (char *) device_state_work;
            }
        }
        
        // Wait for 5 seconds before sending again
        xtimer_usleep(INTERVAL);
    }

    return 0;
}