
#include <net/sock/udp.h>
#include <xtimer.h>
#include <string.h>
#include <net/ipv4/addr.h>
#include <net/sock/util.h>
#include "MQTTClient.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t server_ep = SOCK_IPV4_EP_ANY;

void init_udp_socket(void) {
    sock_udp_ep_t local_ep = SOCK_IPV4_EP_ANY;
    local_ep.port = 0;  // Let the system assign a local port

    server_ep.family = AF_INET;
    server_ep.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &server_ep.addr.ipv4);

    if (sock_udp_create(&sock, &local_ep, &server_ep, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }
}

void publish_state(void) {
    char payload[PAYLOAD_LEN];
    memcpy(payload, PAYLOAD, PAYLOAD_LEN);

    if (sock_udp_send(&sock, payload, PAYLOAD_LEN, &server_ep) < 0) {
        puts("Error sending UDP packet");
    } else {
        puts("State published");
    }
}

int main(void) {
    init_udp_socket();

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
