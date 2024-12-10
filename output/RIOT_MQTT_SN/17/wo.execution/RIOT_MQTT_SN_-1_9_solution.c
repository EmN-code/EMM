#include <xtimer.h> 
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <msg.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>

#define EMCUTE_PORT     (1883U)
#define EMCUTE_ID       ("esp32-client")
#define SERVER_IP       ("47.102.103.1")
#define TOPIC_NAME      ("device/state")
#define PUB_INTERVAL    (5U)
#define STATE_WORKING   ("work")
#define STATE_NOT_WORKING ("not work")

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_MAIN];

// Message queue
static msg_t queue[8];

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL; // should never be reached
}

int main(void) {
    // Setup a message queue
    msg_init_queue(queue, ARRAY_SIZE(queue));

    // Start the emcute thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    // Set remote gateway address
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    if (inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4) != 1) {
        puts("Error: Unable to parse server address");
        return 1;
    }

    // Connect to MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: Unable to connect to MQTT-SN broker");
        return 1;
    }

    // Register topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: Unable to register topic");
        return 1;
    }

    // Publish status every PUB_INTERVAL seconds
    while (1) {
        // Toggle state
        static bool work = true;
        const char *state = work ? STATE_WORKING : STATE_NOT_WORKING;

        if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: Unable to publish data");
        }
        xtimer_sleep(PUB_INTERVAL);

        // Toggle state for next publish
        work = !work;
    }

    return 0;
}
