
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/tcp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define BUF_SIZE 128
#define INTERVAL 5000000  // 5 seconds in microseconds

static char buf[BUF_SIZE];
static sock_tcp_t sock;

void publish_state(void) {
    const char *state = "work";  // or "not work" based on your logic
    int len = snprintf(buf, BUF_SIZE, "PUBLISH state %s\n", state);

    if (sock_tcp_write(&sock, buf, len) < 0) {
        puts("Error writing to socket");
    }
}

int main(void) {
    ipv4_addr_t server_addr;
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    if (inet_pton(AF_INET, SERVER_IP, &server_addr) <= 0) {
        puts("Error converting IP address");
        return 1;
    }

    remote.addr.ipv4 = server_addr;

    if (sock_tcp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating socket");
        return 1;
    }

    if (sock_tcp_connect(&sock, &remote, 0) < 0) {
        puts("Error connecting to server");
        sock_tcp_close(&sock);
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    sock_tcp_close(&sock);
    return 0;
}
