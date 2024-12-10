
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL_MS 5000000

static char state[] = "work";

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    sock_udp_t sock;
    int result;

    // Initialize the remote endpoint
    remote.port = SERVER_PORT;
    result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Error converting IP address");
        return 1;
    }

    // Initialize the UDP socket
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        // Send the device state
        if (sock_udp_send(&sock, state, strlen(state), &remote) < 0) {
            puts("Error sending UDP packet");
        } else {
            printf("Sent: %s\n", state);
        }

        // Wait for 5 seconds
        xtimer_usleep(INTERVAL_MS);
    }

    // Close the socket
    sock_udp_close(&sock);

    return 0;
}
