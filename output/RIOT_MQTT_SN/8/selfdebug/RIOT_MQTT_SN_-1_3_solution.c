#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <net/ipv6/addr.h>  // for inet_pton
#include "net/emcute.h"
#include "xtimer.h"

#define EMCUTE_ID           "device001"
#define EMCUTE_PORT         (1883U)
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_DEFAULT_TOPIC_ID 1
#define PUBLISH_INTERVAL    (5000000U)  // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_MAIN];  // Define a stack for the thread
static emcute_topic_t subscription_topic;
static emcute_topic_t publish_topic;

void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL;
}

int main(void) {
    // Initialize the network stack
    printf("[MQTT-SN] Connecting to broker at 47.102.103.1:%d\n", EMCUTE_PORT);

    // Initialize and start the EMCUTE thread
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    // MQTT-SN configuration
    sock_udp_ep_t gw = { .family = AF_INET6 }; // Using IPv6, replace AF_INET6 with AF_INET if you use IPv4
    inet_pton(AF_INET6, "47.102.103.1", &gw.addr);
    gw.port = EMCUTE_PORT;

    emcute_con(&gw, true, NULL, NULL, 0, 0);

    // Define topic for publishing
    publish_topic.name = "state";
    emcute_reg(&publish_topic);

    while (1) {
        // publish state
        const char *message = "work";
        if (emcute_pub(&publish_topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("[MQTT-SN] Error: unable to publish\n");
        }
        printf("[MQTT-SN] Published: %s\n", message);

        // Sleep for publishing interval
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    emcute_discon();
    return 0;
}