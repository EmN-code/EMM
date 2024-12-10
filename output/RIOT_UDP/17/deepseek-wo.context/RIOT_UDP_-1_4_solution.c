
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
    char buf[16];  // Buffer to hold the device state message

    // Convert the server IP address from string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error: Invalid server IP address");
        return 1;
    }

    remote.port = SERVER_PORT;

    // Initialize the UDP socket
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error: Failed to create UDP socket");
        return 1;
    }

    while (1) {
        // Prepare the device state message
        strncpy(buf, DEVICE_STATE, sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';  // Ensure null-termination

        // Send the device state message to the server
        if (sock_udp_send(&sock, buf, strlen(buf), &remote) < 0) {
            puts("Error: Failed to send UDP message");
        } else {
            printf("Sent: %s\n", buf);
        }

        // Wait for 5 seconds before sending the next message
        xtimer_usleep(INTERVAL);
    }

    // Close the UDP socket
    sock_udp_close(&sock);

    return 0;
}
