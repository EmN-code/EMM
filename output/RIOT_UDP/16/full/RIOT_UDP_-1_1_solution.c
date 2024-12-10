#include "thread.h"
#include "sock/udp.h"
#include "xtimer.h"
#include "net/af.h"
#include "stdio.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

int main(void) {
    uint8_t buf[64];
    char *message = "work";
    sock_udp_ep_t server = { .family = AF_INET6, .netif = SOCK_ADDR_ANY_NETIF, .port = SERVER_PORT };
    sock_udp_ep_t remote;

    // Convert server IP address
    if (ipv6_addr_from_str(&(server.addr.ipv6), SERVER_IP) == NULL) {
        printf("Error converting server address\n");
        return 1;
    }

    // Initialize the remote
    server.family = AF_INET6;

    // UDP socket descriptor
    sock_udp_t sock;

    // Attempt to connect to the server
    while (1) {
        if (sock_udp_create(&sock, &remote, &server, 0) < 0) {
            printf("Error creating UDP socket\n");
            xtimer_sleep(1);
            continue;
        }
        else {
            printf("Connected to server\n");
            break; // Connection successful
        }
    }

    // Main loop to send UDP messages
    while (1) {
        if (sock_udp_send(&sock, message, strlen(message), &server) < 0) {
            printf("Error sending message\n");
        } else {
            printf("Message sent: %s\n", message);
        }

        xtimer_usleep(SEND_INTERVAL); // Wait for the interval before sending next
    }

    return 0;
}