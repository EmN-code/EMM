
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>  // Include this for inet_pton

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static const char *states[] = {"work", "not work"};
static int current_state = 0;

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    char buf[64];

    // Initialize the remote endpoint
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Create a UDP socket
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        // Prepare the message
        snprintf(buf, sizeof(buf), "Device state: %s", states[current_state]);

        // Send the message
        if (sock_udp_send(&sock, buf, strlen(buf), &remote) < 0) {
            puts("Error sending UDP message");
        } else {
            printf("Sent: %s\n", buf);
        }

        // Toggle the state
        current_state = !current_state;

        // Wait for 5 seconds
        xtimer_usleep(INTERVAL);
    }

    // Close the socket (this part is unreachable in this example)
    sock_udp_close(&sock);

    return 0;
}
