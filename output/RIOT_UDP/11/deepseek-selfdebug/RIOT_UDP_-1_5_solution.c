
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = SERVER_PORT;
    remote.family = AF_INET;

    // Convert IP address string to byte representation
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error: Invalid server IP address");
        return 1;
    }

    // Create a UDP socket
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error: Failed to create UDP socket");
        return 1;
    }

    char buf[64];
    while (1) {
        // Prepare the message
        snprintf(buf, sizeof(buf), "Device state: %s", DEVICE_STATE);

        // Send the message
        if (sock_udp_send(&sock, buf, strlen(buf), NULL) < 0) {
            puts("Error: Failed to send UDP message");
        } else {
            printf("Sent: %s\n", buf);
        }

        // Wait for 5 seconds
        xtimer_usleep(INTERVAL);
    }

    // Close the socket
    sock_udp_close(&sock);

    return 0;
}