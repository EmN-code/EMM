#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"

#define EMCUTE_PRIO  (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS    (16U)
#define TOPIC_MAXLEN (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_sub_t subscriptions[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(CONFIG_EMCUTE_DEFAULT_PORT, ".");
    return NULL; // This should never be reached.
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));

    /* Start emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    /* Set up connection to MQTT-SN broker */
    sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to [%s]:%d\n", "47.102.103.1", 1883);
        return 1;
    }

    /* Publish message every 5 seconds */
    emcute_topic_t topic;
    topic.name = "state";

    while (1) {
        int res = emcute_reg(&topic);
        if (res != EMCUTE_OK) {
            puts("error: unable to obtain topic ID\n");
            return 1;
        }

        const char *data = "work"; // or "not work"
        emcute_pub(&topic, data, strlen(data), EMCUTE_QOS_0);

        xtimer_sleep(5);
    }

    /* We never reach here */
    emcute_discon();
    return 0;
}