#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "xtimer.h"

#define EMCUTE_ID         "esp32_mqtt"
#define EMCUTE_PORT       (1883U)
#define EMCUTE_PRIO       (THREAD_PRIORITY_MAIN - 1)
#define STATE_TOPIC       "device/state"
#define PUBLISH_INTERVAL  (5 * 1000000U)  // 5 seconds

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static emcute_topic_t topic;

static void *emcute_thread(void *arg) {
    (void) arg;
    msg_init_queue(queue, ARRAY_SIZE(queue));
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL;
}

static int connect_to_broker(const sock_udp_ep_t *gw) {
    return emcute_con((sock_udp_ep_t *)gw, true, NULL, NULL, 0, 0);
}

static int publish_to_topic(const char *state) {
    unsigned flags = EMCUTE_QOS_0;
    return emcute_pub(&topic, state, strlen(state), flags);
}

static void update_state_and_publish(void) {
    const char *state = "work";  // or "not work"
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return;
    }

    while (1) {
        if (publish_to_topic(state) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        }
        xtimer_usleep(PUBLISH_INTERVAL);
    }
}

int main(void) {
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    sock_udp_ep_t gw = {
        .family = AF_INET6,
        .port = EMCUTE_PORT
    };

    if (inet_pton(AF_INET6, "47.102.103.1", &gw.addr.ipv6) != 1) {
        puts("Error: unable to parse gateway address\n");
        return 1;
    }

    if (connect_to_broker(&gw) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway\n");
        return 1;
    }

    strncpy(topic.name, STATE_TOPIC, sizeof(topic.name));
    topic.name[sizeof(topic.name) - 1] = '\0';  // Ensure null-termination

    printf("Connected to MQTT-SN broker, starting to publish state...\n");
    update_state_and_publish();

    return 0;
}