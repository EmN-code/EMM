#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <xtimer.h>
#include <thread.h>
#include <msg.h>

#define EMCUTE_PORT (1883U)
#define EMCUTE_ID "riot-esp32"
#define EMCUTE_PRIO (THREAD_PRIORITY_MAIN - 1)
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_TOPIC "device/state"
#define PUBLISH_INTERVAL (5U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static emcute_topic_t topic;

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
}

void publish_state(void) {
    char *state = "work";
    unsigned flags = EMCUTE_QOS_0;
    printf("Publishing state: %s\n", state);
    emcute_pub(&topic, state, strlen(state), flags);
}

static void *emcute_thread(void *arg) {
    (void)arg;
    msg_init_queue(queue, ARRAY_SIZE(queue));
    emcute_run(EMCUTE_PORT, EMCUTE_ID);

    return NULL;
}

int main(void) {
    /* Start the emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    /* Connect to the broker */
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Failed to connect to broker\n");
        return 1;
    }
    
    /* Set up the topic we will publish on */
    topic.name = MQTT_TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Failed to register topic\n");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}