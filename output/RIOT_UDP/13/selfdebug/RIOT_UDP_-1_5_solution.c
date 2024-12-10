#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <xtimer.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * 1000000U) // Send every 5 seconds

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &server.addr);

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return -1;
    }

    char buffer[32];
    const char *device_state = "work"; // The device state to send

    while (1) {
        // Use the buffer now
        snprintf(buffer, sizeof(buffer), "Device state: %s", device_state);
        
        if (sock_udp_send(&sock, buffer, strlen(buffer), &server) < 0) {
            puts("Error sending UDP packet");
        } else {
            puts("UDP packet sent");
        }
        
        xtimer_usleep(INTERVAL); // Sleep for the defined interval
    }

    // Should never be reached in this example
    sock_udp_close(&sock);
    return 0;
}