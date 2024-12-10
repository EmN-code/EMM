#include "net/ipv6/addr.h"
#include "net/emcute.h"
#include "msg.h"
#include <stdio.h>
#include <string.h>

#define EMCUTE_ID       "esp32-node"
#define EMCUTE_PORT     (1883U)
#define TOPIC_NAME      "state"
#define TOPIC_DATA      "work"
#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)
#define MQTT_INTERVAL   (5U)

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static void on_receive(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
    // handle received message
}

static void *emcute_thread(void *arg) {
    msg_init_queue(queue, ARRAY_SIZE(queue));
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL;
}

int main(void) {
    // Initialize the emcute library
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    // Set up the endpoint for the MQTT-SN broker
    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = EMCUTE_PORT
    };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%d\n", "47.102.103.1", EMCUTE_PORT);
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to obtain a topic ID");
        return 1;
    }

    // The state message to be published
    const char *state_message = TOPIC_DATA;

    // Publish every 5 seconds
    while (1) {
        if (emcute_pub(&topic, state_message, strlen(state_message), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("error: unable to publish data\n");
        }
        xtimer_sleep(MQTT_INTERVAL);
    }

    return 0;
}