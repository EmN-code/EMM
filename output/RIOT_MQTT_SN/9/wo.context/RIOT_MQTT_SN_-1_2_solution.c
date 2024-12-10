#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <xtimer.h>
#include <net/ipv6/addr.h>
#include <emcute.h>

#define EMCUTE_ID          "my_riot_node"
#define EMCUTE_PORT        1883
#define BROKER_IP          "47.102.103.1"
#define STATE_TOPIC        "state"
#define MSG_INTERVAL       (5U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static emcute_topic_t state_topic;

static void connect(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to [%s]:%d\n", BROKER_IP, EMCUTE_PORT);
        return;
    }
    printf("Connected to [%s]:%d\n", BROKER_IP, EMCUTE_PORT);
}

static void publish_state(const char *state) {
    unsigned flags = EMCUTE_QOS_0;
    if (emcute_pub(EMCUTE_TIT_NORMAL, state, strlen(state), state_topic.id, flags) != EMCUTE_OK) {
        printf("Failed to publish %s topic\n", STATE_TOPIC);
    } else {
        printf("Published: %s\n", state);
    }
}

int main(void) {
    msg_init_queue(queue, 8);
    emcute_run(EMCUTE_PORT, EMCUTE_ID);

    state_topic.name = STATE_TOPIC;
    if (emcute_reg(&state_topic) != EMCUTE_OK) {
        printf("Error: unable to register topic %s\n", STATE_TOPIC);
        return 1;
    }

    connect();

    while (1) {
        publish_state("work");
        xtimer_sleep(MSG_INTERVAL);
        publish_state("not work");
        xtimer_sleep(MSG_INTERVAL);
    }

    return 0;
}