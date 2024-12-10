
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/ipv4/addr.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void publish_state(int sock) {
    const char *state = "work"; // or "not work" based on your logic
    const char *topic = "state";
    char message[64];
    snprintf(message, sizeof(message), "PUBLISH %s %s\n", topic, state);

    ssize_t res = sock_tcp_write(sock, message, strlen(message));
    if (res < 0) {
        puts("Failed to send message");
    } else {
        printf("Published: %s\n", message);
    }
}

int main(void) {
    sock_tcp_t sock;
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote;
    remote.family = AF_INET;
    remote.port = BROKER_PORT;

    if (inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4) <= 0) {
        puts("Invalid address");
        return 1;
    }

    if (sock_tcp_create(&sock, &local, NULL, 0) < 0) {
        puts("Failed to create socket");
        return 1;
    }

    if (sock_tcp_connect(&sock, &remote, 0) < 0) {
        puts("Failed to connect to broker");
        sock_tcp_close(&sock);
        return 1;
    }

    while (1) {
        publish_state(sock.fd);
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    sock_tcp_close(&sock);
    return 0;
}
