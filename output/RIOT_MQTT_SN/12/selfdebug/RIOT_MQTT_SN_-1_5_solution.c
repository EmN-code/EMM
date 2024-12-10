#include "net/emcute.h"
#include "net/sock/udp.h"
#include "random.h"
#include "ztimer.h"
#include <stdio.h>
#include <string.h>

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (1U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_sub_t subscriptions[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];

static emcute_topic_t t;

static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    char *in = (char *)data;
    (void)topic;
    printf("### received publication for "%s" [%i] byte(s): %s\n", topic->name, (int)len, in);
}

static int publish_state(const char *state)
{
    unsigned flags = EMCUTE_QOS_0;

    t.name = "device_status";
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID\n");
        return 1;
    }

    if (emcute_pub(&t, state, strlen(state), flags) != EMCUTE_OK) {
        printf("error: unable to publish data\n");
        return 1;
    }

    printf("published %s on topic %s\n", state, t.name);

    return 0;
}

void *publisher(void *arg)
{
    (void)arg;
    char *states[] = {"work", "not work"};

    while (1) {
        int idx = rand() % 2;
        publish_state(states[idx]);
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void)
{
    puts("RIOT MQTT-SN example\n");

    msg_init_queue(queue, ARRAY_SIZE(queue));

    /* initialize our subscription buffers */
    memset(subscriptions, 0, (NUMOFSUBS * sizeof(emcute_sub_t)));
    memset(topics, 0, (NUMOFSUBS * TOPIC_MAXLEN));

    /* start the emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_run, "emcute", "emcute");

    /* connect to broker */
    sock_udp_ep_t gw = { .family = AF_INET6, .port = atoi("1883")} ;
    char *addr_str = "47.102.103.1";
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, addr_str);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway\n");
        return 1;
    }

    puts("Connected to MQTT-SN broker\n");

    /* start the publisher thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, publisher, NULL, "publisher");

    return 0;
}