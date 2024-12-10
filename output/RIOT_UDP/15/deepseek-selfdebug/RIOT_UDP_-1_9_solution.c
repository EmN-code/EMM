
#include <stdio.h>
#include <string.h>
#include <net/ipv4/addr.h>
#include <net/sock.h>
#include <xtimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL 5000000  // 5 seconds in microseconds

static char buffer[128];
static sock_udp_t sock;
static sock_udp_ep_t remote;

void send_state(const char *state) {
    strncpy(buffer, state, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    if (sock_udp_send(&sock, buffer, strlen(buffer), &remote) < 0) {
        puts("Error sending UDP message");
    } else {
        printf("Sent: %s\n", buffer);
    }
}

int main(void) {
    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error converting IP address");
        return 1;
    }

    // Initialize the UDP socket
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Send device state every 5 seconds
    while (1) {
        send_state("work");
        xtimer_usleep(INTERVAL);
        send_state("not work");
        xtimer_usleep(INTERVAL);
    }

    // Close the socket (this part is unreachable in this example)
    sock_udp_close(&sock);

    return 0;
}
