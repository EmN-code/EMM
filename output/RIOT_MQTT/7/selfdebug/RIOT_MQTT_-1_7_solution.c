#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include <net/ipv6/addr.h>
#include <net/ipv4/addr.h>
#include <net/sock/tcp.h>
#include <net/af.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define INTERVAL (5U * US_PER_SEC) // 5 seconds

static void publish_state(sock_tcp_t *sock) {
    const char *state = "work";
    ssize_t res = sock_tcp_send(sock, state, strlen(state));
    if (res < 0) {
        printf("Failed to send state\n");
    }
    else {
        printf("State sent: %s\n", state);
    }
}

int main(void) {
    printf("MQTT Publisher started\n");
    
    sock_tcp_ep_t remote;
    sock_tcp_t sock;
    char addr_str[IPV6_ADDR_MAX_STR_LEN]; // Allocate enough space for the string representation of the IPv6 address
    
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("inet_pton failed\n");
        return 1;
    }

    remote.port = SERVER_PORT;

    if (sock_tcp_connect(&sock, &remote, sizeof(remote)) < 0) {
        printf("Connection failed\n");
        return 1;
    }

    while (1) {
        publish_state(&sock);
        xtimer_sleep(5);
    }

    sock_tcp_disconnect(&sock);
    return 0;
}