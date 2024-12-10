#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <xtimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "work"
#define INTERVAL (5U * US_PER_SEC) // 5 seconds interval in microseconds

static sock_udp_ep_t remote;

int main(void) {
    // Initialize the remote endpoint.
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;

    // Convert IP address from string to the correct format
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: Could not convert IP address\n");
        return -1;
    }

    sock_udp_t sock;
    // Create the socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: Could not create UDP sock\n");
        return -1;
    }

    char *message = MESSAGE;
    while (1) {
        // Send message every 5 seconds
        ssize_t res = sock_udp_send(&sock, message, strlen(message), &remote);
        if (res < 0) {
            printf("Error sending message\n");
        } else {
            printf("Sent %s\n", message);
        }
        xtimer_sleep(5); // Sleep for 5 seconds
    }

    // Close the socket
    sock_udp_close(&sock);
    return 0;
}