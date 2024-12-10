
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static void send_state(void) {
    sock_udp_t sock;
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    uint8_t buf[128];
    size_t len;

    // Initialize the remote endpoint
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Initialize the UDP socket
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }

    // Prepare the message
    len = snprintf((char *)buf, sizeof(buf), "%s", DEVICE_STATE);

    // Send the message
    if (sock_udp_send(&sock, buf, len, &remote) < 0) {
        puts("Error sending UDP message");
    } else {
        printf("Sent: %s\n", DEVICE_STATE);
    }

    // Close the socket
    sock_udp_close(&sock);
}

int main(void) {
    while (1) {
        send_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
