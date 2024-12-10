#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define EMCUTE_ID "esp32"
#define EMCUTE_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC)
#define MQTT_SN_IP {47, 102, 103, 1}

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

int main(void) {
    // Setup and initialize the network connection
    char *stack = malloc(THREAD_STACKSIZE_DEFAULT);
    if (stack == NULL) {
        puts("Error allocating stack memory.");
        return -1;
    }

    gw.addr.ipv4[0] = MQTT_SN_IP[0];
    gw.addr.ipv4[1] = MQTT_SN_IP[1];
    gw.addr.ipv4[2] = MQTT_SN_IP[2];
    gw.addr.ipv4[3] = MQTT_SN_IP[3];

    emcute_run(EMCUTE_PORT, EMCUTE_ID);

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Could not connect to broker.");
        free(stack);
        return -1;
    }
    topic.name = "state";

    // Publish loop
    while (1) {
        char message[20];
        strcpy(message, "work or not work");

        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish message.");
        } else {
            puts("Published state.");
        }
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }

    // Free resources
    free(stack);
    return 0;
}