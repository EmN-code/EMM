#include <xtimer.h>
#include <net/sock/udp.h>   // Correct include for UDP in RIOT
#include <kernel_types.h>   // Necessary for RIOT types
#include <stdio.h>          // For input and output

#define SERVER_PORT     (20001)
#define CLIENT_PORT     (12345)
#define SEND_PERIOD     (5U)    // Send interval in seconds
#define SERVER_ADDR     "47.102.103.1"

int main(void)
{
    // Buffer for the message
    char message[] = "work or not work";

    // Socket and endpoint variables
    sock_udp_t sock;
    sock_udp_ep_t server = { .family = AF_INET6, .netif = SOCK_ADDR_ANY_NETIF }; // Using IPv6 address format

    // Set up the server address and port
    if (ipv6_addr_from_str((ipv6_addr_t *)&server.addr, SERVER_ADDR) == NULL) {
        puts("Error: unable to parse server address");
        return 1;
    }
    server.port = SERVER_PORT;

    // Create the UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    // Timer setup for periodic sending
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        // Send the message to the server
        if (sock_udp_send(&sock, message, sizeof(message), &server) < 0) {
            puts("Error: unable to send message");
        } else {
            puts("Message sent");
        }

        // Wait for the next period
        xtimer_periodic_wakeup(&last_wakeup, SEND_PERIOD * US_PER_SEC);
    }

    // Close the socket (code never reaches here in this example)
    sock_udp_close(&sock);

    return 0;
}