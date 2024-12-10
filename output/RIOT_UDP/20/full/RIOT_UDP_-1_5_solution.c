#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <xtimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * 1000000U) // 5 seconds

static uint8_t buf[64];

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET, .netif = SOCK_ADDR_ANY_NETIF, .port = SERVER_PORT };
    int rc;

    // Convert IP address from string to binary representation
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: Invalid IP address\n");
        return 1;
    }

    // Initialize socket
    if ((rc = sock_udp_create(&sock, NULL, NULL, 0)) < 0) {
        printf("Error: Failed to create UDP sock: %d\n", rc);
        return 1;
    }

    while(1) {
        // Send device state
        const char *device_state = "work";
        size_t len = snprintf((char *)buf, sizeof(buf), "%s", device_state);
        if ((rc = sock_udp_send(&sock, buf, len, &remote)) < 0) {
            printf("Error: Failed to send data: %d\n", rc);
            xtimer_sleep(1);
            continue;
        } else {
            printf("Sent: %s\n", device_state);
        }

        // Wait for the next sending interval
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}