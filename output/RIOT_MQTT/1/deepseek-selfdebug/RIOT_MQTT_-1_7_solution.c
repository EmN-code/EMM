
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock.h>
#include <net/tcp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define BUF_SIZE 128
#define INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static char buf[BUF_SIZE];
static sock_tcp_t sock;

void publish_state(void) {
    const char *message = "work or not work";
    size_t len = strlen(message);

    if (sock_tcp_write(&sock, message, len) < 0) {
        puts("Failed to send message");
    } else {
        puts("Message sent");
    }
}

int main(void) {
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    if (sock_tcp_create(&sock, &local, &remote, 0) < 0) {
        puts("Failed to create socket");
        return 1;
    }

    if (sock_tcp_connect(&sock, &remote, 0) < 0) {
        puts("Failed to connect to server");
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
