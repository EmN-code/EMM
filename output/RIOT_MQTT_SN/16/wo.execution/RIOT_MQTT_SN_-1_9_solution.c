#include "net/sock/udp.h"
#include "net/emcute.h"
#include "msg.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"

#define EMCUTE_PORT        (1883U)
#define EMCUTE_ID          "riot"
#define EMCUTE_PRIO        (THREAD_PRIORITY_MAIN - 1)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

#define TOPIC_NAME  "state"
#define STATE_WORKING "work"
#define STATE_NOT_WORKING "not work"

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL;
}

int main(void) {
    msg_init_queue(queue, 8);
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Could not connect to MQTT-SN gateway\n");
        return 1;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Failed to register topic\n");
        return 1;
    }

    while (true) {
        if (emcute_pub(&topic, STATE_WORKING, strlen(STATE_WORKING), 0) != EMCUTE_OK) {
            puts("Error publishing data\n");
        }
        xtimer_sleep(5);

        if (emcute_pub(&topic, STATE_NOT_WORKING, strlen(STATE_NOT_WORKING), 0) != EMCUTE_OK) {
            puts("Error publishing data\n");
        }
        xtimer_sleep(5);
    }

    return 0;
}