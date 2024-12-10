
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"

#define EMCUTE_PORT 1883
#define EMCUTE_ID "riot_esp32"
#define TOPIC_NAME "state"
#define STATE_MESSAGE "work"

static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
static emcute_topic_t topic;

static void *emcute_thread(void *arg)
{
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL;
}

static void publish_state(void)
{
    const char *message = STATE_MESSAGE;
    unsigned flags = EMCUTE_QOS_0;

    if (emcute_pub(&topic, message, strlen(message), flags) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        puts("State published");
    }
}

int main(void)
{
    // Initialize the UDP endpoint for the gateway
    IP4_ADDR(&gw.addr.ipv4, 47, 102, 103, 1);

    // Initialize the EMCUTE thread
    thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
