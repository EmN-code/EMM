
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/gnrc/netif.h"
#include "net/gnrc.h"

#define EMCUTE_PORT 1883
#define EMCUTE_ID "riot_esp32"
#define TOPIC "device/state"
#define BROKER_ADDR "47.102.103.1"

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_sub_t sub;

static void *emcute_thread(void *arg)
{
    (void)arg;
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    ipv4_addr_t broker_addr;

    if (ipv4_addr_from_str(&broker_addr, BROKER_ADDR) == NULL) {
        puts("Error: unable to parse broker address");
        return NULL;
    }
    gw.addr.ipv4 = broker_addr;

    emcute_opt_t opts = { .node_id = EMCUTE_ID };
    if (emcute_connect(&gw, &opts) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    while (1) {
        xtimer_sleep(5);
        const char *state = (rand() % 2 == 0) ? "work" : "not work";
        emcute_pub_t pub;
        pub.topic.name = TOPIC;
        pub.topic.size = strlen(TOPIC);
        pub.qos = 0;
        pub.retain = 0;
        pub.msg = state;
        pub.msg_len = strlen(state);

        if (emcute_pub(&pub) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        } else {
            printf("Published: %s\n", state);
        }
    }

    return NULL;
}

int main(void)
{
    puts("RIOT MQTT-SN example");

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    return 0;
}
