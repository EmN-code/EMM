#include <xtimer.h>
#include <stdio.h>
#include <net/sock/udp.h>

#define BUFFER_SIZE 64
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DELAY (5U * US_PER_SEC) // 5 seconds delay

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    char message[] = "work";  // Device state message
    uint8_t buffer[BUFFER_SIZE];
    sock_udp_t sock;

    // Convert IP address from string to network format
    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_IP) == NULL) {
        puts("Error: unable to parse IPv4 address");
        return 1;
    }

    // Create UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    while (1) {
        printf("Sending message: %s\n", message);

        // Send the message to the server
        if (sock_udp_send(&sock, message, sizeof(message), &remote) < 0) {
            puts("Error: unable to send message");
        }

        // Wait for the specified delay
        xtimer_sleep(DELAY);
    }

    return 0;
}