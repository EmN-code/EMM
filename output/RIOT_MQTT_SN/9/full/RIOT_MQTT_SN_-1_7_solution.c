#include <ztimer.h>
#include <xtimer.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <msg.h>
#include <net/emcute.h>
#include <arpa/inet.h>

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static emcute_sub_t subscriptions[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];

void *emcute_thread(void *arg)
{
    (void)arg;
    msg_init_queue(queue, ARRAY_SIZE(queue));
    emcute_run(1883, "client_id");
    return NULL; /* should never be reached */
}

static int publish_state(const char *state) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    memset(&t, 0, sizeof(t));
    snprintf(t.name, sizeof(t.name), "state_topic");

    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID\n");
        return 1;
    }

    if (emcute_pub(&t, state, strlen(state), flags) != EMCUTE_OK) {
        printf("error: unable to publish data\n");
        return 1;
    }

    printf("[PUB] success: %s
", state);
    return 0;
}

int main(void) {
    kernel_pid_t emcute_pid;

    /* initialize emcute thread */
    emcute_pid = thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                               emcute_thread, NULL, "emcute");

    /* set the broker address */
    sock_udp_ep_t gw = {.family = AF_INET, .port = 1883};
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    /* ensure we are connected */
    while (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to gateway\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    
    /* periodic publishing */
    while (1) {
        publish_state("work");
        ztimer_sleep(ZTIMER_SEC, 5);
    }
    
    return 0;
}
