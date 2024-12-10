
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <xtimer.h>
#include <msg.h>
#include <stdbool.h>
#include <timex.h>
#include <mutex.h>
#include <evtimer_msg.h>
#include <dht.h>
#include <dht_params.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <net/ipv4/addr.h>
#include <net/mqttsn.h>
#include <net/emcute.h>
#include <ztimer.h>
#include <evtimer_mbox.h>
#include <paho_mqtt.h>
#include <time_units.h>
#include <stdatomic.h>
#include <kernel_defines.h>
#include <stddef.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t server = SOCK_IPV4_EP_ANY;

void publish_state(void) {
    char payload[] = PAYLOAD;
    sock_udp_send(&sock, payload, PAYLOAD_LEN, &server);
}

int main(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 12345; // Local port

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    server.port = SERVER_PORT;
    if (inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4) <= 0) {
        puts("Error converting IP address");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
