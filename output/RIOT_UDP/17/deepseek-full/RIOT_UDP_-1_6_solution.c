
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <xtimer.h>
#include <stdbool.h>
#include <msg.h>
#include <thread.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE_WORK "work"
#define DEVICE_STATE_NOT_WORK "not work"
#define SEND_INTERVAL (5 * 1000 * 1000) // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t remote;

void send_device_state(const char *state) {
    ssize_t res = sock_udp_send(&sock, state, strlen(state), &remote);
    if (res < 0) {
        puts("Failed to send device state");
    } else {
        printf("Sent device state: %s\n", state);
    }
}

int main(void) {
    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Create the UDP socket
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Failed to create UDP socket");
        return 1;
    }

    while (1) {
        // Send device state "work"
        send_device_state(DEVICE_STATE_WORK);

        // Wait for 5 seconds
        xtimer_usleep(SEND_INTERVAL);

        // Send device state "not work"
        send_device_state(DEVICE_STATE_NOT_WORK);

        // Wait for 5 seconds
        xtimer_usleep(SEND_INTERVAL);
    }

    // Close the socket (this part is unreachable in this loop)
    sock_udp_close(&sock);

    return 0;
}
