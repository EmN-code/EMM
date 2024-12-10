
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };

int main(void)
{
    // Convert IP address string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4) != 1) {
        puts("Error: Invalid server IP address");
        return 1;
    }

    // Create a UDP socket
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, &server, 0) < 0) {
        puts("Error: Failed to create UDP socket");
        return 1;
    }

    char message[20];
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        // Simulate device state
        const char *state = (xtimer_now().ticks32 % 2 == 0) ? "work" : "not work";
        snprintf(message, sizeof(message), "Device state: %s", state);

        // Send the message to the server
        if (sock_udp_send(&sock, message, strlen(message), &server) < 0) {
            puts("Error: Failed to send UDP message");
        } else {
            printf("Sent: %s\n", message);
        }

        // Wait for 5 seconds
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    }

    // Close the socket
    sock_udp_close(&sock);

    return 0;
}
