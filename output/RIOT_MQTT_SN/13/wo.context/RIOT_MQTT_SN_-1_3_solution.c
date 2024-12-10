#include <xtimer.h>
#include <stdio.h>
#include <net/ipv6/addr.h>
#include <arpa/inet.h> // Header file for inet_pton

#include "msg.h"
#include "net/emcute.h"

#define EMCUTE_ID        "mqtt-sn"
#define EMCUTE_PRIO      (THREAD_PRIORITY_MAIN - 1)
#define BROKER_PORT      (1883)
#define PUBLISH_INTERVAL (5U)

static char stack[THREAD_STACKSIZE_DEFAULT]; // Define stack for thread
static msg_t queue[8];

static emcute_topic_t topic;
static char *state_message = "work";

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
}

static int connect_to_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };

    if (inet_pton(AF_INET, "47.102.103.1", &gw.addr) <= 0) {
        puts("Error: unable to parse broker address");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to broker");
        return 1;
    }

    return 0;
}

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(BROKER_PORT, EMCUTE_ID);
    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));
    // Create emcute thread
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    if (connect_to_broker() != 0) {
        return 1;
    }

    memset(&topic, 0, sizeof(topic));
    topic.name = "device/state";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    while (1) {
        if (emcute_pub(&topic, state_message, strlen(state_message), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish data");
        } else {
            printf("Published state: %s\n", state_message);
        }
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}