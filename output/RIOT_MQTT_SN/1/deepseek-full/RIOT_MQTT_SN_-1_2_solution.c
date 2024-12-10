
#include "thread.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/sock/util.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define STACKSIZE 1024
#define PRIORITY 5
#define TOPIC_NAME "state"
#define STATE_MSG "work"
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883

static char stack[STACKSIZE];
static emcute_topic_t topic;
static sock_udp_ep_t broker_ep;

void publish_state(void) {
    while (1) {
        int res = emcute_pub(&topic, STATE_MSG, sizeof(STATE_MSG) - 1, 0);
        if (res != EMCUTE_OK) {
            printf("Failed to publish state: %d\n", res);
        }
        xtimer_sleep(5);
    }
}

int main(void) {
    // Initialize the UDP endpoint for the broker
    broker_ep.family = AF_INET;
    broker_ep.port = BROKER_PORT;
    inet_pton(AF_INET, BROKER_IP, &broker_ep.addr.ipv4);

    // Connect to the MQTT-SN broker
    int res = emcute_con(&broker_ep, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN broker: %d\n", res);
        return 1;
    }

    // Register the topic
    strncpy(topic.name, TOPIC_NAME, sizeof(topic.name));
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Failed to register topic: %d\n", res);
        return 1;
    }

    // Create and start the thread for publishing state
    thread_create(stack, sizeof(stack), PRIORITY, 0, (thread_task_func_t)publish_state, NULL, "publisher");

    return 0;
}
