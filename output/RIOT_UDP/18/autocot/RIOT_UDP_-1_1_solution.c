#include "xtimer.h"
#include "udp.h"
#include "net/af.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC) // Send every 5 seconds

static char *device_state = "work";

int main(void) {
    struct sockaddr_in6 server_addr;
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };
    sock_udp_t sock;

    // Convert the server address string to a network address
    if (ipv6_addr_from_str(&remote.addr.ipv6, SERVER_ADDR) == NULL) {
        printf("Error: unable to parse server address\n");
        return -1;
    }

    // Configure the UDP sock
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: unable to create UDP sock\n");
        return -1;
    }

    while (1) {
        // Send the device state to the server
        if (sock_udp_send(&sock, device_state, strlen(device_state), &remote) < 0) {
            printf("Error: unable to send message\n");
        } else {
            printf("Message sent: %s\n", device_state);
        }

        // Wait for the next send interval
        xtimer_sleep(SEND_INTERVAL / US_PER_SEC);
    }

    return 0;
}