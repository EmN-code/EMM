#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

static emcute_sub_t subscriptions[NUMOFSUBS];
static char stack[THREAD_STACKSIZE_DEFAULT];

static const char *state_work = "work";
static const char *state_not_work = "not work";

static emcute_topic_t state_topic;

void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    char *in = (char *)data;
    in[len] = '\0';    /* ensure null termination */
    printf("Received publication for topic '%s' [%i] : "%s"\n",
           topic->name, topic->id, (char *)data);
}

static void *emcute_thread(void *arg)
{
    (void)arg;
    emcute_run(1883, "riot_client");
    return NULL;
}

static int pub(const char *state)
{
    emcute_sub_t sub;
    sub.topic.name = state_topic.name;
    sub.cb = on_pub;

    unsigned flags = EMCUTE_QOS_0;

    if (emcute_sub(&sub, flags) != EMCUTE_OK) {
        puts("error: unable to subscribe to topic");
        return 1;
    }

    if (emcute_pub(&state_topic, state, strlen(state), flags) != EMCUTE_OK) {
        puts("error: unable to publish data");
        return 1;
    }

    printf("Published %s state\n", state);

    return 0;
}

int main(void)
{
    msg_init_queue(_msg_emcute_queue, sizeof(_msg_emcute_queue) / sizeof(_msg_emcute_queue[0]));

    state_topic.name = "device/state";

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    sock_udp_ep_t gw = { .family = AF_INET6, .port = 1883 };

    if (inet_pton(AF_INET6, "47.102.103.1", &gw.addr.ipv6) != 1) {
        printf("error: unable to parse IPv6 address\n");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0){
        printf("error: unable to connect to gateway at 47.102.103.1\n");
        return 1;
    }

    while (1) {
        pub(state_work);
        xtimer_sleep(5);
    }

    return 0;
}