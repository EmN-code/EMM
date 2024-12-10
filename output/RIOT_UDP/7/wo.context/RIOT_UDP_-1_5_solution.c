#include <xtimer.h>
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>
#include <stdio.h>

#define IP_ADDRESS "47.102.103.1"
#define PORT (20001)
#define SEND_INTERVAL (5U * 1000000U) // 5 seconds in microseconds

// Define the device state
const char *device_state = "work";

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = PORT };

    // Convert IP address from text to binary form
    if (inet_pton(AF_INET, IP_ADDRESS, &remote.addr.ipv4) != 1) {
        puts("Error: unable to convert IP address.");
        return -1;
    }

    // Create a UDP socket
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create UDP sock.");
        return -1;
    }

    // Sending loop
    while (1) {
        ssize_t res = sock_udp_send(&sock, device_state, sizeof(device_state), &remote);
        if (res < 0) {
            printf("Error sending data: %zd\n", res);
        } else {
            puts("Device state sent!");
        }
        
        // Sleep for the defined interval
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}