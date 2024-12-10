#include "net/emcute.h"
#include "xtimer.h"
#include <stdio.h>
#include <string.h>

#define EMCUTE_ID           "gertrud"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define DEFAULT_PORT        (1883U)
#define DEFAULT_INTERVAL    (5U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];
static emcute_topic_t topic;
static char *mqtt_topic = "state";
static sock_udp_ep_t gw = { .family = AF_INET, .port = DEFAULT_PORT };

static void *emcute_thread(void *arg) {
    (void)arg;
    static char publish_msg[20];
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        strcpy(publish_msg, "work");

        /* publish data every 5 seconds */
        xtimer_periodic_wakeup(&last_wakeup, DEFAULT_INTERVAL * US_PER_SEC);
        if (emcute_pub(&topic, publish_msg, strlen(publish_msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish data.");
        }
    }

    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    /* setup gateway address */
    if (inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4) != 1) {
        puts("Error: unable to parse gateway address.");
        return 1;
    }

    /* connect to MQTT-SN gateway */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway.");
        return 1;
    }

    /* setup subscription topic */
    topic.name = mqtt_topic;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to obtain topic ID.");
        return 1;
    }

    return 0;
}