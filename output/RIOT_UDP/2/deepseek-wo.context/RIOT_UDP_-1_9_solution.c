
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static void send_state(sock_udp_t *sock, const sock_udp_ep_t *remote) {
    char buf[16];
    strncpy(buf, DEVICE_STATE, sizeof(buf));
    ssize_t res = sock_udp_send(sock, buf, strlen(buf), remote);
    if (res < 0) {
        printf("Failed to send UDP packet\n");
    } else {
        printf("Sent device state: %s\n", buf);
    }
}

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;

    // Initialize the remote endpoint
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Initialize the UDP socket
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        printf("Failed to create UDP socket\n");
        return 1;
    }

    while (1) {
        send_state(&sock, &remote);
        xtimer_usleep(INTERVAL_MS);
    }

    // Close the socket (this will never be reached in this example)
    sock_udp_close(&sock);
    return 0;
}
