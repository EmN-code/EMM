
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

int main(void)
{
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    sock_udp_t sock;
    char buf[256];
    int res;

    // Convert IP address string to binary form
    res = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (res != 1) {
        puts("Error converting IP address");
        return 1;
    }

    // Initialize UDP socket
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        // Prepare the message
        snprintf(buf, sizeof(buf), "Device state: %s", DEVICE_STATE);

        // Send the message
        if (sock_udp_send(&sock, buf, strlen(buf), NULL) < 0) {
            puts("Error sending UDP message");
        } else {
            printf("Sent: %s\n", buf);
        }

        // Wait for 5 seconds
        xtimer_usleep(INTERVAL_MS);
    }

    // Close the socket (this part is unreachable in this example)
    sock_udp_close(&sock);

    return 0;
}
