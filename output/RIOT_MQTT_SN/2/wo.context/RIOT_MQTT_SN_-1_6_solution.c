#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <net/ipv6/addr.h>
#include <emcute.h>

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_sub_t subscriptions[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];

static const char *state_topic = "device/state";
static const char *msg_working = "work";
static const char *msg_not_working = "not work";

static void *pub_thread(void *arg) {
    (void)arg;
    emcute_topic_t t;
    t.name = state_topic;

    while (1) {
        t.id = emcute_reg(&t);
        if (t.id == 0) {
            printf("[pub] unable to obtain topic ID\n");
            continue;
        }
        
        const char *state_msg = (rand() % 2 == 0) ? msg_working : msg_not_working;
        if (emcute_pub(&t, state_msg, strlen(state_msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("[pub] unable to publish data\n");
        }

        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, pub_thread, NULL, "pub_thread");

    sock_udp_ep_t gw = { .family = AF_INET6, .port = 1883 };

    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, "47.102.103.1") == NULL) {
        printf("error: unable to parse IPv6 address\n");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to gateway\n");
        return 1;
    }

    printf("Connection to MQTT-SN broker established\n");

    return 0;
}