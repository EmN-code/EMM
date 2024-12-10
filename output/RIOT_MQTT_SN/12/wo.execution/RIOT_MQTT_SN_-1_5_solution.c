#include <stdio.h>
#include "net/sock/udp.h" // Ensure correct UDP socket headers
#include "net/emcute.h"  // Include emcute for MQTT-SN
#include "msg.h"         // For message queue

#define EMCUTE_PORT         (1883U)
#define MQTT_SN_ADDRESS     "47.102.103.1"
#define CLIENT_ID           "esp32_client"
#define PUBLISH_INTERVAL    (5U) // seconds
#define TOPIC_NAME          "device/state"

static emcute_topic_t topic;
static char buf[64];
static msg_t queue[8];

static void publish_state(void) {
    const char *state = "work"; // the state to publish
    snprintf(buf, sizeof(buf), "%s", state);
    if (emcute_pub(&topic, buf, strlen(buf), EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error: unable to publish data");
    } else {
        puts("Successfully published data");
    }
}

void *thread_handler(void *arg) {
    (void)arg;
    while (1) {
        publish_state();
        sleep(PUBLISH_INTERVAL);
    }
    return NULL;
}

int main(void) {
    // Initialize message queue
    msg_init_queue(queue, ARRAY_SIZE(queue));

    // Setup server endpoint
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    if (inet_pton(AF_INET, MQTT_SN_ADDRESS, &gw.addr.ipv4) != 1) {
        puts("Error: unable to parse gateway address");
        return 1;
    }

    // Connect to gateway
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway");
        return 1;
    }

    // Register topic
    strcpy(topic.name, TOPIC_NAME);
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    // Start publication thread
    static char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, thread_handler, NULL, "pub_thread");

    // Main loop, keep running
    while (1) {
        sleep(10);
    }
    return 0;
}