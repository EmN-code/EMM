
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>  // Include this header for inet_pton
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    uint8_t buf[128];
    int result;

    // Initialize the UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Convert the server IP address from string to binary form
    result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Error converting IP address");
        return 1;
    }

    // Set the server port
    remote.port = SERVER_PORT;

    while (1) {
        // Prepare the message
        snprintf((char *)buf, sizeof(buf), "Device state: %s", DEVICE_STATE);

        // Send the message to the server
        if (sock_udp_send(&sock, buf, strlen((char *)buf), &remote) < 0) {
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
