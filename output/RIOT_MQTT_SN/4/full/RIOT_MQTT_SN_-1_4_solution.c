#include <ztimer.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <thread.h>
#include <timex.h>
#include <time_units.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <mutex.h>
#include <msg.h>
#include <arpa/inet.h>
#include "MQTTClient.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "dht_params.h"

#define EMCUTE_PORT     (1883U)
#define SERVER_ADDR     "47.102.103.1"
#define TOPIC_NAME      "state"
#define STACK_SIZE      (THREAD_STACKSIZE_DEFAULT)
#define SLEEP_INTERVAL  (5U)

static char stack[STACK_SIZE];  // Stack for the thread
static msg_t queue[8];          // Message queue for the emcute thread
static emcute_topic_t topic;

/**
 * @brief Thread to publish data periodically.
 */
static void *publisher_thread(void *arg) {
    (void)arg;

    while (1) {
        const char *state = "work";  // State to publish
        if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        }

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_SEC, SLEEP_INTERVAL);
    }
    return NULL;
}

/**
 * @brief Initialize the emcute connection.
 */
static int init_emcute(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));

    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    if (inet_pton(AF_INET, SERVER_ADDR, &gw.addr.ipv4) <= 0) {
        printf("Error: unable to parse IP address\n");
        return -1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to gateway\n");
        return -1;
    }

    // Register topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return -1;
    }

    return 0;
}

int main(void) {
    puts("RIOT MQTT-SN publisher");

    // Initialize emcute
    if (init_emcute() != 0) {
        return -1;
    }

    // Start the publisher thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publisher_thread, NULL, "publisher");

    // Main thread idle loop
    while (1) {
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;
}
