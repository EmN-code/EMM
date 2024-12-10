#include <stdio.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <xtimer.h>

#define NUMOFSUBS 16
#define TOPIC_MAXLEN 64

static emcute_sub_t subscriptions[NUMOFSUBS];
static char stack[THREAD_STACKSIZE_DEFAULT];

// MQTT broker connection details
#define MQTT_BROKER "47.102.103.1"
#define MQTT_PORT "1883"

// Message publication interval
#define PUB_INTERVAL (5U * US_PER_SEC)

// IPv6 address of the broker
static sock_udp_ep_t gw = {
    .family = AF_INET6,
    .port = MQTT_PORT
};

void *emcute_thread(void *arg) {
    (void)arg;
    // Initialize the EMCUTE stack
    printf("Initializing emcute network\n");
    if (emcute_sock(&gw, sizeof(gw)) != EMCUTE_OK) {
        printf("Error: unable to initialize emcute\n");
        return NULL;
    }

    // Connect to the MQTT-SN broker
    printf("Connecting to broker at %s\n", MQTT_BROKER);
    if (emcute_con("test-client", true) != EMCUTE_OK) {
        printf("Error: unable to connect to broker at %s\n", MQTT_BROKER);
        return NULL;
    }

    // Register the topic on the MQTT-SN broker
    emcute_topic_t topic;
    topic.name = "device/state";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic 'device/state'\n");
        return NULL;
    }

    // Publish loop
    while (1) {
        const char *state = "work";
        // Attempt to publish
        int res = emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Error: unable to publish state\n");
        }
        else {
            printf("State published successfully: %s\n", state);
        }

        // Wait for the next publication
        xtimer_usleep(PUB_INTERVAL);
    }
    return NULL;
}

int main(void) {
    // Start the emcute thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_WOUT_YIELD | THREAD_CREATE_STACKTEST,
                  emcute_thread, NULL, "emcute");

    return 0;
}