
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define INTERVAL_MS 5000000  // 5 seconds in microseconds

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = UDP_SERVER_PORT };
    sock_udp_t sock;
    char buf[] = DEVICE_STATE;

    // Convert IP address from string to binary form
    if (sock_udp_str2ep(&remote, UDP_SERVER_IP) < 0) {
        puts("Error: unable to parse destination address");
        return 1;
    }

    // Initialize UDP socket
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    while (1) {
        // Send the device state to the UDP server
        if (sock_udp_send(&sock, buf, strlen(buf), NULL) < 0) {
            puts("Error: unable to send UDP message");
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
