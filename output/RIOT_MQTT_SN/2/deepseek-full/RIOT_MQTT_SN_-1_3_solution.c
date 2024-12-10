
#include "thread.h"
#include "net/sock/udp.h"
#include "mma8x5x_regs.h"
#include "net/gcoap.h"
#include "net/sock/util.h"
#include "mutex.h"
#include "mma8x5x.h"
#include "net/emcute.h"
#include "arpa/inet.h"
#include "net/ipv6/addr.h"

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define STATE_MESSAGE "work or not work"
#define EMCUTE_BUFSIZE 128

static emcute_topic_t topic;
static sock_udp_ep_t gw;

static void _on_pub(void *arg, int res) {
    (void)arg;
    if (res != EMCUTE_OK) {
        puts("Error on publish");
    }
}

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };

    inet_aton(BROKER_ADDR, &remote.addr.ipv4);

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    gw.family = AF_INET;
    gw.port = BROKER_PORT;
    gw.addr.ipv4 = remote.addr.ipv4;

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return 1;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }

    while (1) {
        if (emcute_pub(&topic, STATE_MESSAGE, sizeof(STATE_MESSAGE) - 1, 0) != EMCUTE_OK) {
            puts("Error publishing message");
        }
        thread_sleep(5 * 1000 * 1000); // Sleep for 5 seconds
    }

    return 0;
}
