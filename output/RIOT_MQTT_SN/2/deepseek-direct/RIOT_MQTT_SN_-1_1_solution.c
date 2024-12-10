
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/gnrc/netif.h"
#include "net/gnrc/netapi.h"
#include "net/gnrc/netreg.h"
#include "net/gnrc/udp.h"
#include "net/gnrc/ipv6.h"
#include "net/gnrc/tcp.h"
#include "net/gnrc/sixlowpan.h"
#include "net/gnrc/sixlowpan/nd.h"
#include "net/gnrc/sixlowpan/frag.h"
#include "net/gnrc/sixlowpan/iphc.h"
#include "net/gnrc/sixlowpan/rpl.h"
#include "net/gnrc/sixlowpan/rpl/dodag.h"
#include "net/gnrc/sixlowpan/rpl/dodag_conf.h"
#include "net/gnrc/sixlowpan/rpl/dodag_conf_default.h"
#include "net/gnrc/sixlowpan/rpl/dodag_conf_default_dodag.h"
#include "net/gnrc/sixlowpan/rpl/dodag_conf_default_dodag_conf.h"
#include "net/gncute.h"

#define EMCUTE_PORT       1883
#define EMCUTE_PRIO       1
#define EMCUTE_TOPIC      "device/state"
#define EMCUTE_QOS        0
#define EMCUTE_MSG_LEN    20
#define EMCUTE_INTERVAL   (5 * 1000000U) /* 5 seconds in microseconds */

static char emcute_stack[THREAD_STACKSIZE_DEFAULT];
static char emcute_msg[EMCUTE_MSG_LEN];
static emcute_topic_t emcute_topic;

static void *emcute_thread(void *arg)
{
    (void)arg;

    sock_udp_ep_t local = { .family = AF_INET6, .port = EMCUTE_PORT };
    sock_udp_ep_t remote = { .family = AF_INET6, .port = EMCUTE_PORT, .addr = { .ipv6 = { 0x2f, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x10, 0x21, 0x03 } } };

    if (sock_udp_create(&emcute_sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return NULL;
    }

    emcute_topic_t topics[] = {
        { .name = EMCUTE_TOPIC, .qos = EMCUTE_QOS }
    };

    if (emcute_sub(&emcute_topic, topics, 1) != EMCUTE_OK) {
        puts("Error subscribing to topic");
        return NULL;
    }

    while (1) {
        snprintf(emcute_msg, EMCUTE_MSG_LEN, "work");
        emcute_pub(&emcute_topic, emcute_msg, strlen(emcute_msg), EMCUTE_QOS);
        xtimer_usleep(EMCUTE_INTERVAL);
        snprintf(emcute_msg, EMCUTE_MSG_LEN, "not work");
        emcute_pub(&emcute_topic, emcute_msg, strlen(emcute_msg), EMCUTE_QOS);
        xtimer_usleep(EMCUTE_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    puts("Starting MQTT-SN client");

    thread_create(emcute_stack, sizeof(emcute_stack), THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    return 0;
}
