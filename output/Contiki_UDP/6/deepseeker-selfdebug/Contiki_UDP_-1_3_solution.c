
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>

#define UDP_SERVER_ADDR "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define SEND_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

int main(void)
{
    sock_udp_ep_t server = { .family = AF_INET, .port = UDP_SERVER_PORT };
    sock_udp_t sock;
    char buf[16];

    // Resolve the server address
    if (sock_udp_str2ep(&server, UDP_SERVER_ADDR) < 0) {
        puts("Error resolving server address");
        return 1;
    }

    // Initialize the UDP socket
    if (sock_udp_create(&sock, NULL, &server, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        // Prepare the message
        snprintf(buf, sizeof(buf), "%s", DEVICE_STATE);

        // Send the message
        if (sock_udp_send(&sock, buf, strlen(buf), &server) < 0) {
            puts("Error sending UDP message");
        } else {
            printf("Sent: %s\n", buf);
        }

        // Wait for 5 seconds
        xtimer_usleep(SEND_INTERVAL);
    }

    // Close the socket
    sock_udp_close(&sock);

    return 0;
}
