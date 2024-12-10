
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/ipv6/addr.h>
#include <xtimer.h>
#include <thread.h>
#include <msg.h>
#include <net/emcute.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_QUEUE_SIZE 8
#define STACK_SIZE 1024

static msg_t msg_queue[MSG_QUEUE_SIZE];
static char stack[STACK_SIZE];

static void publish_state(void) {
    static const char *states[] = {"work", "not work"};
    static int state_index = 0;

    emcute_topic_t topic;
    topic.name = TOPIC_NAME;

    // Register the topic if not already registered
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return;
    }

    // Publish the state
    if (emcute_pub(&topic, states[state_index], strlen(states[state_index]), 0) != EMCUTE_OK) {
        puts("Error: unable to publish state");
    } else {
        printf("Published: %s\n", states[state_index]);
    }

    // Toggle state
    state_index = (state_index + 1) % 2;
}

static void *mqtt_thread(void *arg) {
    (void)arg;

    sock_udp_ep_t gw = { .port = EMQX_SERVER_PORT };
    inet_pton(AF_INET, EMQX_SERVER_IP, &gw.addr.ipv4);

    // Connect to the MQTT-SN gateway
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN gateway");
        return NULL;
    }

    puts("Connected to MQTT-SN gateway");

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Create the MQTT thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, mqtt_thread, NULL, "mqtt");

    return 0;
}
