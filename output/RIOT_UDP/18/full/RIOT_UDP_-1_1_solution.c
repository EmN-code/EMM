#include <xtimer.h> // Timer utilities
#include <stdio.h>  // Standard I/O functions
#include <net/af.h>  // Address Family definitions (e.g., AF_INET)
#include <net/protnum.h>  // Protocol number definitions
#include <net/sock/udp.h>  // UDP socket definitions

#define SERVER_ADDR "47.102.103.1"  // UDP server address
#define SERVER_PORT 20001  // UDP server port
#define INTERVAL 5  // Message sending interval in seconds

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t server = { .family = AF_INET, .netif = SOCK_ADDR_ANY_NETIF };
    int rc;

    // Convert server address IP string to sockaddr
    if (ipv6_addr_from_str((ipv6_addr_t *)&server.addr, SERVER_ADDR) == NULL) {
        puts("Failed to parse server address");
        return 1;
    }
    server.port = SERVER_PORT;

    // Create UDP socket
    while (1) {
        rc = sock_udp_create(&sock, NULL, NULL, 0);
        if (rc != 0) {
            printf("Create socket failed: %d\n", rc);
            xtimer_sleep(1);
            continue;
        }
        break;
    }

    while (1) {
        char *message = "work";

        // Send message to the server
        rc = sock_udp_send(&sock, message, strlen(message), &server);
        if (rc < 0) {
            printf("Send failed: %d\n", rc);
        } else {
            printf("Message sent: %s\n", message);
        }

        // Wait for INTERVAL seconds before sending the next message
        xtimer_sleep(INTERVAL);
    }
    return 0;
}