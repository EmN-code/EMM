#include <ztimer/ztimer.h>
#include <thread.h>
#include <stdio.h>
#include <string.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define EMCUTE_ID "riot_node"
#define PUB_TOPIC "device/state"

static char stack[THREAD_STACKSIZE_MAIN]; // Stack for the main thread

// Function to publish state to the MQTT-SN broker
static int pub_state(char *state) {
    emcute_topic_t topic;
    topic.name = PUB_TOPIC;
    topic.id = 0; // Will be assigned by emcute_reg
    int ret = emcute_reg(&topic);
    if (ret != EMCUTE_OK) {
        puts("Failed to register topic");
        return ret;
    }

    ret = emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
    if (ret != EMCUTE_OK) {
        puts("Failed to publish message");
    }
    return ret;
}

int main(void) {
    emcute_sub_t subscription;
    emcute_topic_t topic;

    // Connect to the broker
    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = BROKER_PORT
    };

    inet_pton(AF_INET, BROKER_IP, &gw.addr);
    int ret = emcute_con(&gw, true, NULL, NULL, 0, EMCUTE_CS);
    if (ret != EMCUTE_OK) {
        puts("Error: Unable to connect to MQTT-SN broker");
        return 1;
    }

    // Run the emcute receiver
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_run, NULL, "emcute");

    // Publish state every 5 seconds
    while (1) {
        pub_state("work");
        ztimer_sleep(ZTIMER_SEC, 5);
        pub_state("not work");
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}