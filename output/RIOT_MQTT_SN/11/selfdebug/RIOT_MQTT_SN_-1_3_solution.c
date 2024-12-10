#include <string.h>
#include <stdio.h>
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"
#include "net/emcute.h"
#include "ztimer.h"

#define EMCUTE_ID           "esp32"
#define EMCUTE_PORT         CONFIG_EMCUTE_DEFAULT_PORT
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define BROKER_IP           "47.102.103.1"
#define TOPIC_NAME          "state"
#define TOPIC_ID            1

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL;
}

int main(void) {
    /* Initialize MQTT-SN */
    memset(&topic, 0, sizeof(emcute_topic_t));
    topic.name = TOPIC_NAME;
    topic.id = TOPIC_ID;

    /* Start the emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    /* Define the gateway address */
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    /* Connect to the broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to broker\n");
        return 1;
    }
    puts("Successfully connected to broker\n");

    /* Prepare the data to be published */
    const char *state = "work";  // Alternately publish "work" or "not work"

    /* Publish state every 5 seconds */
    while (1) {
        if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("error: unable to publish data\n");
        } else {
            puts("Data published!");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000); // Sleep for 5 seconds
        state = (strcmp(state, "work") == 0) ? "not work" : "work";  // Toggle between states
    }

    return 0;
}