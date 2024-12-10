#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define MQTT_SN_ADDRESS "47.102.103.1"
#define MQTT_SN_PORT 1883
#define TOPIC_NAME "state"
#define INTERVAL (5U * US_PER_SEC)  // 5 seconds

static emcute_sock_t sock;
static emcute_sub_t subscriptions[1];

static char stack[THREAD_STACKSIZE_DEFAULT];

/**
 * @brief Publishes a message to a specified topic.
 */
static void publish_state(const char *state) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    t.name = TOPIC_NAME;

    if (emcute_reg(&t) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return;
    }

    if (emcute_pub(&t, state, strlen(state), flags) != EMCUTE_OK) {
        printf("Error: unable to publish data\n");
    } else {
        printf("Published %s every %d seconds\n", state, INTERVAL / US_PER_SEC);
    }
}

/**
 * @brief Thread function to publish state periodically.
 *
 * @param arg Unused
 */
static void *publisher_thread(void *arg) {
    (void)arg;
    char *state_work = "work";
    char *state_notwork = "not work";
    
    while (1) {
        publish_state(state_work);
        xtimer_sleep(INTERVAL / US_PER_SEC);
        publish_state(state_notwork);
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    return NULL;
}

int main(void) {
    memset(subscriptions, 0, (sizeof(emcute_sub_t) * ARRAY_SIZE(subscriptions)));

    sock = emcute_socket();

    // Define the remote broker address
    sock.family = AF_INET6;
    if (ipv6_addr_from_str((ipv6_addr_t *)&sock.add, MQTT_SN_ADDRESS) == NULL) {
        printf("Error: unable to parse broker address\n");
        return 1;
    }
    sock.port = MQTT_SN_PORT;

    // Connect to the broker
    if (emcute_con(sock) != EMCUTE_OK) {
        printf("Error: unable to connect to [%s]:%d\n", MQTT_SN_ADDRESS, MQTT_SN_PORT);
        return 1;
    }

    // Start the periodic publishing thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publisher_thread, NULL, "publisher");

    return 0;
}