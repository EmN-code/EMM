#include <string.h>
#include <net/ipv6/addr.h> // inet_pton is usually defined here in RIOT for both IPv4 and IPv6
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>  

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001

static sock_udp_ep_t server = {.family = AF_INET, .port = SERVER_PORT};

int main(void) {
    uint8_t state = 1; // 1 represents "work", 0 represents "not work"
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);

    // Configure the socket
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: unable to create UDP socket\n");
        return 1;
    }

    while (1) {
        // Send the device state to the server
        char buffer[2];
        snprintf(buffer, sizeof(buffer), "%d", state);
        if (sock_udp_send(&sock, buffer, sizeof(buffer), &server) < 0) {
            printf("Error: failed to send UDP message\n");
        }
        else {
            printf("Device state sent: %s\n", buffer);
        }
        
        // Wait for 5 seconds before sending the next state
        xtimer_sleep(5);

        // Toggle device state for next message
        state = !state;
    }

    return 0;
}