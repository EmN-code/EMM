#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <thread.h>
#include <net/mqttsn.h>

#define EMCUTE_PORT     CONFIG_ASYMCUTE_DEFAULT_PORT
#define MQTT_TOPIC_NAME "status"
#define MQTT_CLIENT_ID  "client"

#ifndef EMCUTE_PRIO
#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)
#endif

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    printf("[info] Received publication: %.*s\n", (int)len, (char *)data);
}

static void *emcute_thread(void *arg) {
    emcute_run(EMCUTE_PORT, MQTT_CLIENT_ID);
    return NULL;
}

int main(void) {
    sock_udp_ep_t gw = {.port = EMCUTE_PORT};

    gw.family = AF_INET;
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    /* Initialize emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    /* Connect to the broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("[error] Unable to connect to broker\n");
        return 1;
    }
    printf("[info] Connected to broker\n");

    /* Register the topic */
    topic.name = MQTT_TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("[error] Unable to register topic\n");
        return 1;
    }
    printf("[info] Registered topic '%s'\n", topic.name);

    while (1) {
        const char *state = "work";
        if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("[error] Unable to publish\n");
        } else {
            printf("[info] Published state: %s\n", state);
        }
        xtimer_sleep(5);
    }

    return 0;
}
