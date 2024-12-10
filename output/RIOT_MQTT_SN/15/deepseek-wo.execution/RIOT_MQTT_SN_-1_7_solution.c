
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>
#include <thread.h>
#include <xtimer.h>
#include <msg.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_QUEUE_SIZE 8
#define STACK_SIZE 1024

static char stack[STACK_SIZE];
static msg_t msg_queue[MSG_QUEUE_SIZE];

static void publish_state(void) {
    static const char *states[] = {"work", "not work"};
    static uint8_t state_index = 0;

    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    topic.id = 0; // Topic ID will be assigned by the gateway

    // Register the topic
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: could not register topic");
        return;
    }

    // Publish the state
    if (emcute_pub(&topic, states[state_index], strlen(states[state_index]), 0) != EMCUTE_OK) {
        puts("Error: could not publish message");
    } else {
        puts("State published");
    }

    // Toggle state
    state_index = (state_index + 1) % 2;
}

static void *mqtt_thread(void *arg) {
    (void)arg;

    sock_udp_ep_t gw = { .family = AF_INET, .port = EMQX_SERVER_PORT };
    inet_pton(AF_INET, EMQX_SERVER_IP, &gw.addr.ipv4);

    // Connect to the MQTT-SN gateway
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: could not connect to MQTT-SN gateway");
        return NULL;
    }

    puts("Connected to MQTT-SN gateway");

    // Initialize the message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    // Create the MQTT thread
    kernel_pid_t mqtt_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                          THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt");

    if (mqtt_pid <= KERNEL_PID_UNDEF) {
        puts("Error: could not create MQTT thread");
        return 1;
    }

    return 0;
}
