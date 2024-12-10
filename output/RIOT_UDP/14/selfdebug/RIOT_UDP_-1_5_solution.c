#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC) // 5 seconds

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET,
                             .port = SERVER_PORT };
    uint8_t buf[128];
    const char *state_msg = "work";
    
    // Convert server IP address to binary form
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error converting server IP");
        return 1;
    }

    // Create the UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        // Send device state to the server
        ssize_t res = sock_udp_send(&sock, state_msg, strlen(state_msg), &remote);
        if (res < 0) {
            printf("Error sending packet: %d\n", (int)res);
        } else {
            puts("State message sent");
        }

        // Wait for the next sending interval
        xtimer_usleep(SEND_INTERVAL);
    }

    // Close the socket (unreachable code)
    sock_udp_close(&sock);
    return 0;
}