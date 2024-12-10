
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE_LEN 12
#define SEND_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char device_state[DEVICE_STATE_LEN] = "work";

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    sock_udp_t sock;
    char buf[DEVICE_STATE_LEN];

    // Convert IP address from string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error: invalid server IP address");
        return 1;
    }

    // Initialize UDP socket
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error: failed to create UDP socket");
        return 1;
    }

    while (1) {
        // Copy device state to buffer
        strncpy(buf, device_state, DEVICE_STATE_LEN - 1);
        buf[DEVICE_STATE_LEN - 1] = '\0';  // Ensure null-termination

        // Send device state to server
        if (sock_udp_send(&sock, buf, strlen(buf), NULL) < 0) {
            puts("Error: failed to send UDP packet");
        } else {
            printf("Sent: %s\n", buf);
        }

        // Wait for 5 seconds
        xtimer_usleep(SEND_INTERVAL);
    }

    // Close UDP socket
    sock_udp_close(&sock);

    return 0;
}
