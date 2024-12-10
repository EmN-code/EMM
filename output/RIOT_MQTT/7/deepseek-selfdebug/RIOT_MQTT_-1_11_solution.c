
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
static sock_tcp_ep_t remote;

void publish_state(void) {
    const char *state = "work";  // or "not work" based on your logic
    int res;

    res = sock_tcp_send(&sock, state, strlen(state), &remote);
    if (res < 0) {
        puts("Error sending data");
    } else {
        printf("Published state: %s\n", state);
    }
}

int main(void) {
    int res;

    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    res = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (res != 1) {
        puts("Error converting IP address");
        return 1;
    }

    res = sock_tcp_create(&sock, NULL, &remote, SOCK_FLAGS_REUSE_EP);
    if (res < 0) {
        puts("Error creating socket");
        return 1;
    }

    res = sock_tcp_connect(&sock, &remote, 0);
    if (res < 0) {
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
