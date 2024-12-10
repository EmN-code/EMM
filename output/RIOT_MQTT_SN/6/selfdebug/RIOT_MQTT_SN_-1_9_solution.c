#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "xtimer.h"
#include "msg.h"
#include "emcute.h"
#include "net/emcute.h"

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];
static emcute_sub_t subscriptions[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(1883, "emcute_client");
    return NULL; /* should never be reached */
}

static int pub(const char *topic, const char *data, int qos)
{
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    /* get topic id */
    for (unsigned i = 0; i < NUMOFSUBS; i++) {
        if (strcmp(topics[i], topic) == 0) {
            t.name = topics[i];
            break;
        }
    }

    t.name = topic;
    t.id = 0; /* if unknown, using 0 will make emcute create a topic ID for us */

    if (qos == 1) {
        flags = EMCUTE_QOS_1;
    }
    else if (qos == 2) {
        flags = EMCUTE_QOS_2;
    }

    if (emcute_pub(&t, data, strlen(data), flags) != EMCUTE_OK) {
        puts("error: unable to publish data");
        return 1;
    }

    return 0;
}

int main(void)
{
    /* initialize our subscription buffers */
    memset(subscriptions, 0, (NUMOFSUBS * sizeof(emcute_sub_t)));
    for (unsigned i = 0; i < NUMOFSUBS; i++) {
        subscriptions[i].topic.name = topics[i];
    }

    /* setup our subscription to the topic */
    strcpy(topics[0], "state");

    /* start emcute thread */
    msg_init_queue(queue, ARRAY_SIZE(queue));
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    /* publish data every 5 seconds */
    while (1) {
        pub("state", "work", 0);
        xtimer_sleep(5);
        pub("state", "not work", 0);
        xtimer_sleep(5);
    }

    return 0;
}