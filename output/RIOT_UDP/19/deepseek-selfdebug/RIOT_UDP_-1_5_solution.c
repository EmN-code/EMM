
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "xtimer.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL 5000000  // 5 seconds in microseconds

static char state_msg[] = "work";

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    struct in_addr server_addr;

    // Convert IP address from string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &server_addr) <= 0) {
        puts("Error: Invalid server IP address");
        return 1;
    }

    // Set up the remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.addr.ipv4 = server_addr;

    // Initialize the UDP socket
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error: Failed to create UDP socket");
        return 1;
    }

    while (1) {
        // Send the state message
        if (sock_udp_send(&sock, state_msg, strlen(state_msg), &remote) < 0) {
            puts("Error: Failed to send UDP message");
        } else {
            printf("Sent: %s\n", state_msg);
        }

        // Wait for 5 seconds
        xtimer_usleep(INTERVAL);
    }

    // Close the socket (this part is unreachable in this example)
    sock_udp_close(&sock);

    return 0;
}