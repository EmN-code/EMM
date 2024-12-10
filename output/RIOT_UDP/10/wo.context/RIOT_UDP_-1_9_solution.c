#include <xtimer.h> /* Import xtimer library for timing functions */
#include <net/sock/udp.h> /* Import UDP socket functionalities */
#include <net/af.h> /* Import address family definitions */
#include <stdio.h> /* Import standard I/O functions */

#define SERVER_ADDRESS "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC) /* send every 5 seconds */
#define MESSAGE "work or not work"

static sock_udp_ep_t server;

int main(void)
{
    /* Initialize the server endpoint structure */
    server.family = AF_INET6; // Choose socket address family, AF_INET6 allows use with both IPv4 and IPv6, change if necessary
    server.netif = SOCK_ADDR_ANY_NETIF;
    server.port = SERVER_PORT;

    /* Convert the string representation of the server address to binary */
    if (inet_pton(AF_INET, SERVER_ADDRESS, &server.addr.ipv4) != 1) {
        puts("Error: Could not convert IP address");
        return 1;
    }

    /* Create a UDP socket */
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, &server, NULL) < 0) {
        puts("Error: Could not create UDP socket");
        return 1;
    }

    while (1) {
        /* Send the message to the server */
        if (sock_udp_send(&sock, MESSAGE, sizeof(MESSAGE), &server) < 0) {
            puts("Error: Could not send message");
        } else {
            puts("Message sent");
        }

        /* Sleep for 5 seconds before sending the next message */
        xtimer_sleep(SEND_INTERVAL / US_PER_SEC);
    }

    return 0; /* This line is never reached */
}
