#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xtimer.h"

#include "net/gnrc/netif.h"
#include "net/gnrc.h"
#include "net/sock/udp.h"
#include "net/af.h"
#include "net/sock.h"
#include "net/mqttsn.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define PUBLISH_INTERVAL (5U)
#define PUB_TOPIC "state"

#define WORK_STATE "work"
#define NOT_WORK_STATE "not work"

static int publish(sock_udp_ep_t *remote) {
    static int counter = 0;
    const char *state = (counter % 2 == 0) ? WORK_STATE : NOT_WORK_STATE;
    counter++;

    mqttsn_pub(NULL, PUB_TOPIC, state, strlen(state), MQTT_QOS_0);
    printf("Published: %s\n", state);

    return 0;
}

int main(void) {
    printf("MQTT Publisher
");

    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    if (gnrc_netif_ipv4_set(add_ipv4_gnrc_netif_ipv4_get(src_iface), &remote.addr) != 0) {
        printf("Setting remote address failed
");
        return 1;
    }

    while (1) {
        publish(&remote);
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}
