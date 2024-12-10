
#include <net/sock/tcp.h>
#include <net/emcute.h>
#include <net/sock.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000 * 1000) // 5 seconds in microseconds

static sock_tcp_t tcp_sock;

void publish_state(void) {
    char payload[10];
    strcpy(payload, PAYLOAD);

    sock_tcp_send(&tcp_sock, payload, strlen(payload));
}

int main(void) {
    sock_ip_ep_t remote;
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error: invalid address");
        return 1;
    }

    if (sock_tcp_create(&tcp_sock, NULL, &remote, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error: failed to create TCP socket");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    sock_tcp_close(&tcp_sock);
    return 0;
}
