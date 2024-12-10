#include <ztimer.h>
#include <xtimer.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "net/emcute.h"
#include "thread.h"
#include "arpa/inet.h"
#include <posix/netinet/in.h>
#include "event/thread.h"


#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)
#define PUBLISH_INTERVAL    (5U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_con(encut_thread, utc, true, NULL, NULL, 0, 0);

static emcute_topic_t topic;
static char *topic_name = "devices/state";
static char *broker_ip = "47.102.103.1";
uint16_t port = 1883;

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(port, "emcute-client");
    return NULL;
}

static int connect_to_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = port };
    if (inet_pton(AF_INET, broker_ip, &gw.addr.ipv4) <= 0) {
        puts("Error translating broker IP");
        return 1;
    }

    while (true) {
        int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (res == EMCUTE_OK) {
            printf("Connected to the broker successfully\n");
            return 0;
        } else {
            printf("Failed to connect to broker: %d\n", res);
            xtimer_sleep(1);
        }
    }
}

static int publish_msg(const char *state) {
    topic.name = topic_name;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Unable to register topic");
        return 1;
    }

    while (true) {
        int res = emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
        if (res == EMCUTE_OK) {
            printf("Published state: %s\n", state);
            return 0;
        } else {
            printf("Failed to publish state: %d\n", res);
            xtimer_sleep(1);
        }
    }
}

int main(void) {
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute_thread");
    
    if (connect_to_broker() != 0) {
        puts("Exiting due to connection failure");
        return 1;
    }

    while (true) {
        publish_msg("work");
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}