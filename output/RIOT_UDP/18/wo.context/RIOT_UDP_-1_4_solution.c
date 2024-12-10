#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>  // Corrected header for UDP functionality
#include <xtimer.h>        // Timer for sleep function

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SLEEP_INTERVAL 5   // Interval in seconds

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    if (inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4) != 1) {
        printf("Error converting server IP address\n");
        return -1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error creating UDP socket\n");
        return -1;
    }

    while (1) {
        const char *device_state = "work";  // Example state, change as needed
        if (sock_udp_send(&sock, device_state, strlen(device_state), &server) < 0) {
            printf("Error sending UDP message\n");
        } else {
            printf("Sent device state: %s\n", device_state);
        }

        xtimer_sleep(SLEEP_INTERVAL);
    }

    // Clean up sock resource (unreachable in this loop, added for proper practice)
    sock_udp_close(&sock);
    return 0;
}