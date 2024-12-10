#include "sys/socket.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/af.h"
#include "net/gcoap.h"

#include <string.h>
#include <unistd.h> // For usleep

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SLEEP_INTERVAL_USEC (5 * 1000 * 1000) // 5 seconds

// Define a function for sending UDP packets
void send_udp_state(sock_udp_t *sock, sock_udp_ep_t *remote, const char *state) {
    size_t len = strlen(state);
    ssize_t res = sock_udp_send(sock, state, len, remote);
    if (res < 0) {
        printf("Error sending UDP packet: %d\n", res);
    }
    else {
        printf("Sent device state: %s\n", state);
    }
}

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    remote.port = SERVER_PORT;

    // Initialize the UDP socket
    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        printf("Error creating UDP socket\n");
        return 1;
    }

    // Device state
    const char *device_state = "work"; // Change to "not_work" as needed.

    while (1) {
        send_udp_state(&sock, &remote, device_state);
        usleep(SLEEP_INTERVAL_USEC);
    }

    sock_udp_close(&sock);
    return 0;
}