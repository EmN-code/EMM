
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/ipv4/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define BUF_SIZE 1024
#define INTERVAL 5000000  // 5 seconds in microseconds

static char send_buf[BUF_SIZE];
static sock_tcp_t sock;

void publish_state(void) {
    const char *state = "work";  // or "not work" based on your logic
    int len = snprintf(send_buf, BUF_SIZE, "PUBLISH state %s\n", state);

    if (sock_tcp_send(&sock, send_buf, len) < 0) {
        puts("Failed to send data");
    } else {
        puts("State published successfully");
    }
}

int main(void) {
    struct sock_tcp_ep remote;
    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Invalid server IP address");
        return 1;
    }

    if (sock_tcp_create(&sock, NULL, &remote, 0) < 0) {
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
