
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/af.h"
#include "net/conn/udp.h"
#include "net/sock/udp.h"
#include "net/netif.h"
#include "shell.h"

#define EMCUTE_PORT       1883
#define EMCUTE_ID         "riot_esp32"
#define EMCUTE_TOPIC      "state"
#define EMCUTE_MSG_WORK   "work"
#define EMCUTE_MSG_NOT_WORK "not work"
#define PUBLISH_INTERVAL  (5 * 1000000U) /* 5 seconds in microseconds */

static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
static emcute_sub_t sub;
static char stack[THREAD_STACKSIZE_DEFAULT];

static void *emcute_thread(void *arg)
{
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL;
}

static void publish_state(void)
{
    static int state = 0;
    const char *msg = state ? EMCUTE_MSG_WORK : EMCUTE_MSG_NOT_WORK;
    state = !state;

    emcute_topic_t t;
    t.name = EMCUTE_TOPIC;
    t.id = 0; /* topic ID will be assigned by the broker */

    if (emcute_pub(&t, msg, strlen(msg), 0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        printf("Published: %s\n", msg);
    }
}

int main(void)
{
    /* Initialize network interface */
    netif_t *netif = netif_iter(NULL);
    if (!netif) {
        puts("Error: no network interface found");
        return 1;
    }

    /* Set gateway address */
    if (ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, "47.102.103.1") == NULL) {
        puts("Error: unable to parse gateway address");
        return 1;
    }

    /* Start the EMCUTE thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    /* Connect to the broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    /* Publish state every 5 seconds */
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
