
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL 5000000  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t remote;

void send_state(const char *state) {
    char buf[20];
    snprintf(buf, sizeof(buf), "%s", state);
    ssize_t res = sock_udp_send(&sock, buf, strlen(buf), &remote);
    if (res < 0) {
        printf("Error sending data\n");
    } else {
        printf("Sent: %s\n", buf);
    }
}

int main(void) {
    // Initialize the remote endpoint
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    // Convert IP address from string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Invalid address/ Address not supported \n");
        return 1;
    }

    // Create a UDP socket
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        printf("Error creating UDP socket\n");
        return 1;
    }

    while (1) {
        send_state("work");
        xtimer_usleep(INTERVAL);
        send_state("not work");
        xtimer_usleep(INTERVAL);
    }

    // Close the socket (this will never be reached in this example)
    sock_udp_close(&sock);

    return 0;
}