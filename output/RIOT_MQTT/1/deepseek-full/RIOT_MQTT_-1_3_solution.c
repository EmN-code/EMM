
#include <arpa/inet.h>
#include <net/sock.h>
#include <net/sock/util.h>
#include <net/ipv4/addr.h>
#include <net/emcute.h>
#include <dht_params.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
static sock_udp_ep_t remote;

void publish_state(void)
{
    char payload[] = PAYLOAD;
    ssize_t res = sock_udp_send(&sock, payload, sizeof(payload) - 1, &remote);
    if (res < 0) {
        printf("Failed to send message: %d\n", (int)res);
    } else {
        printf("Message sent successfully\n");
    }
}

int main(void)
{
    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = EMQX_SERVER_PORT;
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Invalid address\n");
        return 1;
    }

    // Create the UDP socket
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        printf("Failed to create socket\n");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
