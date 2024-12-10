
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/ipv4/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define BUF_SIZE 128
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char buf[BUF_SIZE];
static sock_tcp_t sock;

void publish_state(void) {
    const char *state = "work";  // or "not work" based on your logic
    int res;

    res = sock_tcp_send(&sock, state, strlen(state), 0);
    if (res < 0) {
        printf("Failed to send data: %d\n", res);
    } else {
        printf("Published state: %s\n", state);
    }
}

int main(void) {
    sock_tcp_ep_t remote;
    int res;

    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    res = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (res != 1) {
        printf("Failed to convert IP address\n");
        return 1;
    }

    // Create a TCP socket
    res = sock_tcp_create(&sock, NULL, &remote, 0);
    if (res < 0) {
        printf("Failed to create socket: %d\n", res);
        return 1;
    }

    // Connect to the server
    res = sock_tcp_connect(&sock, &remote, 0);
    if (res < 0) {
        printf("Failed to connect: %d\n", res);
        sock_tcp_close(&sock);
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    // Close the socket
    sock_tcp_close(&sock);
    return 0;
}
