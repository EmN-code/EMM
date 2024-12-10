
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock.h>
#include <net/sock/tcp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define BUF_SIZE 128
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char buf[BUF_SIZE];
static sock_tcp_t sock;

void publish_state(void) {
    const char *state = "work or not work";
    int res;

    res = sock_tcp_send(&sock, state, strlen(state));
    if (res < 0) {
        puts("Error sending data");
    } else {
        printf("Published: %s\n", state);
    }
}

int main(void) {
    sock_tcp_ep_t remote = SOCK_IPV4_EP_ANY;
    int res;

    // Convert IP address string to binary form
    res = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (res != 1) {
        puts("Error converting IP address");
        return 1;
    }
    remote.port = SERVER_PORT;

    // Create a TCP socket
    res = sock_tcp_create(&sock, NULL, &remote, 0);
    if (res < 0) {
        puts("Error creating socket");
        return 1;
    }

    // Connect to the MQTT broker
    res = sock_tcp_connect(&sock, &remote, SOCK_NO_TIMEOUT);
    if (res < 0) {
        puts("Error connecting to server");
        sock_tcp_close(&sock);
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    // Close the socket
    sock_tcp_close(&sock);
    return 0;
}
