#include <ztimer.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <net/af.h>

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static emcute_sub_t subscriptions[NUMOFSUBS];
static char *topics[NUMOFSUBS];

static void *emcute_thread(void *arg) {
    (void)arg;
    msg_init_queue(queue, ARRAY_SIZE(queue));
    emcute_run(EMCUTE_PORT, stack, sizeof(stack), EMCUTE_PRIO);
    return NULL;
}

int main(void) {
    puts("RIOT MQTT-SN publisher example\n");

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    sock_udp_ep_t gateway = { .family = AF_INET6, .port = 1883 };
    inet_pton(AF_INET6, "47.102.103.1", &gateway.addr.ipv6);

    while (1) {
        char *state = "work";
        emcute_topic_t topic;
        topic.name = "state";
        int8_t pub_res = emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
        if (pub_res != EMCUTE_OK) {
            printf("Error: unable to publish data!\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}