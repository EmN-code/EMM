#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "net/emcute.h"
#include "thread.h"

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define STATE_TOPIC_NAME    "state"
#define DEFAULT_BROKER_IP   "47.102.103.1"
#define DEFAULT_BROKER_PORT 1883

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];
static emcute_topic_t state_topic;

void *mqtt_thread(void *arg) {
    sock_udp_ep_t gateway = {.family = AF_INET6, .port = DEFAULT_BROKER_PORT};
    inet_pton(AF_INET6, DEFAULT_BROKER_IP, &gateway.addr.ipv6);

    if (emcute_con(&gateway, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Failed to connect to broker.");
        return NULL;
    }

    state_topic.name = STATE_TOPIC_NAME;
    if (emcute_reg(&state_topic) != EMCUTE_OK) {
        puts("Failed to obtain topic ID.");
        emcute_discon();
        return NULL;
    }

    while (1) {
        const char *state = "work";
        if (emcute_pub(&state_topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish data
");
        }
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(queue, 8);
    emcute_run(DEFAULT_BROKER_PORT, "mqtt-sn");
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, mqtt_thread, NULL, "mqttsn");

    /* Let the main thread idle */
    while (1) {
        thread_yield();
    }
    return 0;
}