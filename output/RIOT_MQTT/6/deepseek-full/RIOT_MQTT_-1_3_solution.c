
#include <net/sock.h>
#include <net/sock/udp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <net/ipv4/addr.h>
#include <net/sock/util.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC "device/state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
static sock_udp_ep_t remote;

static void init_udp_socket(void) {
    local.port = 12345;  // Arbitrary local port
    remote.family = AF_INET;
    remote.port = EMQX_SERVER_PORT;
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }
}

static void publish_state(void) {
    const char *payload = PAYLOAD;
    size_t payload_len = strlen(payload);

    if (sock_udp_send(&sock, payload, payload_len, &remote) < 0) {
        puts("Error sending UDP packet");
        return;
    }

    puts("State published");
}

int main(void) {
    init_udp_socket();

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
