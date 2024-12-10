
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/inet_pton.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

int main(void)
{
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;
    sock_udp_t sock;
    char buffer[256];
    int result;

    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Error converting IP address");
        return 1;
    }

    // Initialize the socket
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        // Prepare the message
        snprintf(buffer, sizeof(buffer), "Device state: %s", DEVICE_STATE);

        // Send the message
        if (sock_udp_send(&sock, buffer, strlen(buffer), &remote) < 0) {
            puts("Error sending UDP message");
        } else {
            printf("Sent: %s\n", buffer);
        }

        // Wait for 5 seconds
        xtimer_usleep(INTERVAL_MS);
    }

    // Close the socket
    sock_udp_close(&sock);

    return 0;
}
