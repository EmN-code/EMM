
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"

#define EMCUTE_PORT 1883
#define EMCUTE_ID "riot_esp32"
#define TOPIC_NAME "state"
#define MSG_SIZE 128

static char emcute_stack[THREAD_STACKSIZE_DEFAULT];
static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

void publish_state(void) {
    static const char *states[] = {"work", "not work"};
    static uint8_t state_index = 0;
    char msg[MSG_SIZE];

    snprintf(msg, sizeof(msg), "%s", states[state_index]);
    state_index = (state_index + 1) % 2;

    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    topic.id = 0; // Topic ID will be assigned by the broker

    if (emcute_reg(&topic) == EMCUTE_OK) {
        if (emcute_pub(&topic, msg, strlen(msg), 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        }
    } else {
        puts("Error: unable to register topic");
    }
}

int main(void) {
    // Initialize the network interface
    netif_t *netif = netif_iter(NULL);
    if (!netif) {
        puts("Error: no network interface found");
        return 1;
    }

    // Set the gateway address
    IPV4_ADDR(&gw.addr.ipv4, 47, 102, 103, 1);

    // Initialize emcute thread
    thread_create(emcute_stack, sizeof(emcute_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_run, &EMCUTE_PORT, "emcute");

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
