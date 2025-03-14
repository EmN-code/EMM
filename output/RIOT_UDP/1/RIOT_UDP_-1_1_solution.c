
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_ep_t server = SOCK_IPV4_EP_ANY;
static sock_udp_t sock;

void send_state(const char *state)
{
    char buf[32];
    sprintf(buf, "Device state: %s", state);
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    if (sock_udp_send(&sock, buf, strlen(buf), &remote) < 0) {
        puts("Error sending UDP message");
    } else {
        printf("Sent: %s\n", buf);
    }
}

int main(void)
{
    // Initialize the UDP socket
    if (sock_udp_create(&sock, NULL, &server, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        send_state("work");
        xtimer_usleep(INTERVAL_MS);
        send_state("not work");
        xtimer_usleep(INTERVAL_MS);
    }

    // Close the UDP socket
    sock_udp_close(&sock);

    return 0;
}
