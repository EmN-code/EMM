
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/af.h"
#include "net/conn/udp.h"
#include "net/netif.h"
#include "shell.h"
#include "msg.h"

#define EMCUTE_PORT       1883
#define EMCUTE_ID         "riot_esp32"
#define TOPIC_NAME        "state"
#define PUBLISH_INTERVAL  (5 * 1000000U)  // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t msg_queue[8];

static emcute_topic_t topic;
static emcute_sub_t sub;

static void *emcute_thread(void *arg)
{
    (void)arg;
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    ipv4_addr_t gw_addr = IPV4_ADDR(47, 102, 103, 1);
    gw.addr.ipv4 = gw_addr;

    if (ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, "47.102.103.1") == NULL) {
        puts("Error: unable to parse gateway address");
        return NULL;
    }

    if (emcute_run(EMCUTE_ID, &gw) != EMCUTE_OK) {
        puts("Error: unable to run EMCUTE");
        return NULL;
    }

    if (emcute_reg(&topic, TOPIC_NAME) != EMCUTE_OK) {
        puts("Error: unable to register to the topic");
        return NULL;
    }

    while (1) {
        char payload[20];
        snprintf(payload, sizeof(payload), "work");
        if (emcute_pub(&topic, payload, strlen(payload), 0) != EMCUTE_OK) {
            puts("Error: unable to publish");
        }
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  emcute_thread, NULL, "emcute");

    return 0;
}
