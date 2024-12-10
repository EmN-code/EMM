// Include header files - MQTTClient.h must be last
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "net/emcute.h"  // Include emcute library header
#include "net/sock/udp.h"
#include "xtimer.h"
#include "thread.h"
#include "MQTTClient.h"   // Must be last

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "riot_client"
#define PUBLISH_TOPIC "device/state"
#define BUFFER_SIZE 128
#define SLEEP_INTERVAL (5 * 1000000U)  // 5 seconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t topic;

void publish_state(void *arg) {
    (void)arg;
    unsigned flags = EMCUTE_QOS_0;
    const char *state = "work";

    emcute_topic_t t;
    t.name = PUBLISH_TOPIC;

    while (1) {
        printf("Publishing state: %s\n", state);
        if (emcute_pub(&t, state, strlen(state), flags) != EMCUTE_OK) {
            puts("Error: unable to publish state");
        }
        printf("Going to sleep for %d seconds\n", SLEEP_INTERVAL / 1000000U);
        xtimer_sleep(SLEEP_INTERVAL / 1000000U);
    }
}

int main(void) {
    // Initialize the emcute stack
    emcute_topic_t connect_topic;
    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, SERVER_ADDR);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Failed to connect to broker");
        return 1;
    }

    // Create a new thread to handle publishing
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publish_state, NULL, "publisher");

    return 0;
}