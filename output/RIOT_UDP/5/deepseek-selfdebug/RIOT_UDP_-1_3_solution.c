
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    char buf[256];
    int result;

    // Initialize the UDP socket
    result = sock_udp_create(&sock, NULL, &remote, 0);
    if (result < 0) {
        printf("Error creating UDP socket: %d\n", result);
        return 1;
    }

    // Convert the server IP address from string to binary form
    result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Error converting IP address: %d\n", result);
        return 1;
    }

    remote.port = SERVER_PORT;

    while (1) {
        // Prepare the message to send
        snprintf(buf, sizeof(buf), "Device state: %s", DEVICE_STATE);

        // Send the message to the server
        result = sock_udp_send(&sock, buf, strlen(buf), &remote);
        if (result < 0) {
            printf("Error sending UDP message: %d\n", result);
        } else {
            printf("Message sent: %s\n", buf);
        }

        // Wait for 5 seconds before sending the next message
        xtimer_usleep(INTERVAL);
    }

    // Close the socket (this part is unreachable in this example)
    sock_udp_close(&sock);

    return 0;
}
