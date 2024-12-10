#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/af.h>  // For address family
#include <net/sock/udp.h>  // For UDP socket operations

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE_DELAY 5  // Delay in seconds

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };
    int rc;
    sock_udp_t sock;
    char message[] = "work";
    
    // Attempts to resolve server address
    rc = ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_IP);
    if (rc == NULL) {
        printf("Error: unable to resolve server address\n");
        return 1;
    }

    // Setup UDP
    while (1) {
        rc = sock_udp_create(&sock, NULL, NULL, 0);
        if (rc != 0) {
            printf("Failed to create UDP sock, retrying in 1s...\n");
            xtimer_sleep(1);
            continue;
        }
        else {
            printf("UDP sock created successfully\n");
            break;
        }
    }

    // Enter an infinite loop to send device state every 5 seconds
    while (1) {
        // Send the message to the server
        rc = sock_udp_send(&sock, message, sizeof(message), &remote);
        if (rc < 0) {
            printf("Failed to send data, retrying...\n");
            continue;
        }
        else {
            printf("Successfully sent: %s\n", message);
        }

        // Wait for the defined delay
        xtimer_sleep(MESSAGE_DELAY);
    }

    // Clean up (the code should not reach here)
    sock_udp_close(&sock);
    return 0;
}