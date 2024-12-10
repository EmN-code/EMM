
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t server = SOCK_IPV6_EP_ANY;

void send_state(const char *state)
{
    char buf[32];
    sprintf(buf, "Device state: %s", state);
    sock_udp_send(&sock, buf, strlen(buf), &server);
}

int main(void)
{
    // Initialize the UDP socket
    sock_udp_create(&sock, NULL, NULL, 0);

    // Set the server endpoint
    server.family = AF_INET;
    server.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);

    // Send device state every 5 seconds
    while (1) {
        send_state("work");
        xtimer_usleep(INTERVAL_MS);
        send_state("not work");
        xtimer_usleep(INTERVAL_MS);
    }

    // Close the socket (this will never be reached in this example)
    sock_udp_close(&sock);

    return 0;
}
