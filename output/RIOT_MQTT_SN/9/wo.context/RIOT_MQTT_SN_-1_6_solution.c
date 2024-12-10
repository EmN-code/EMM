#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shell.h"
#include "thread.h"
#include "xtimer.h"
#include "net/emcute.h"
#include "net/sock/udp.h"

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static emcute_sub_t subscriptions[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];

static sock_udp_ep_t remote = { .family = AF_INET6, .port = 1883 };

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    printf("Received publication of length %u\n", (unsigned)len);
}

static int publish(char *state) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    /* Step 1: Register the topic */
    strcpy(topics[0], "work_state");
    t.name = topics[0];

    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("error: unable to register topic");
        return 1;
    }

    /* Step 2: Publish the message */
    if (emcute_pub(&t, state, strlen(state), flags) != EMCUTE_OK) {
        puts("error: unable to publish data");
        return 1;
    }

    puts("Published state!");
    return 0;
}

static void *thread_handler(void *arg) {
    (void)arg;
    char *state = "work";

    while(1) {
        publish(state);
        xtimer_sleep(5);
        state = (strcmp(state, "work") == 0) ? "not work" : "work";
    }
    return NULL;
}

int main(void) {
    /* Initialize emcute stack */
    msg_init_queue(queue, ARRAY_SIZE(queue));

    /* Connect to the broker */
    memset(&remote.addr, 0, sizeof(remote.addr));
    if (inet_pton(AF_INET6, "47.102.103.1", &remote.addr) == 0) {
        puts("error: unable to parse broker address");
        return 1;
    }

    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to broker");
        return 1;
    }

    /* Start the publisher thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, thread_handler, NULL, "pub_thread");

    /* Run the main emcute loop */
    emcute_run(1883, "riot-esp32");

    return 0;
}