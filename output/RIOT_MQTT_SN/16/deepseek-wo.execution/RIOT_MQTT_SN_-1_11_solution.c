
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <thread.h>
#include <msg.h>

#define MQTT_SN_BROKER_ADDR "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_QUEUE_SIZE 8
#define STACK_SIZE 1024

static msg_t msg_queue[MSG_QUEUE_SIZE];
static char stack[STACK_SIZE];

void publish_state(void) {
    emcute_topic_t topic;
    const char *state = "work";
    int res;

    while (1) {
        // Register the topic if not already registered
        if (topic.id == 0) {
            topic.name = TOPIC_NAME;
            res = emcute_reg(&topic);
            if (res != EMCUTE_OK) {
                printf("Error registering topic: %d\n", res);
                return;
            }
        }

        // Publish the state
        res = emcute_pub(&topic, state, strlen(state), 0);
        if (res != EMCUTE_OK) {
            printf("Error publishing state: %d\n", res);
        } else {
            printf("State published: %s\n", state);
        }

        // Sleep for 5 seconds
        sleep(5);
    }
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_SN_BROKER_PORT };
    int res;

    // Initialize the message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Set the gateway address
    if (ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, MQTT_SN_BROKER_ADDR) == NULL) {
        printf("Error parsing gateway address\n");
        return 1;
    }

    // Connect to the MQTT-SN broker
    res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to MQTT-SN broker: %d\n", res);
        return 1;
    }

    // Create a thread to publish the state
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, (thread_task_func_t)publish_state, NULL, "publisher");

    return 0;
}
