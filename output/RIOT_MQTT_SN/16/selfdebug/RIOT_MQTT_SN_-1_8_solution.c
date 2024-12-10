#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include "msg.h"
#include "emcute.h"
#include "net/ipv4/addr.h"

#define EMCUTE_ID         "my_mqttsn_client"
#define EMCUTE_PRIO       (THREAD_PRIORITY_MAIN - 1)
#define MQTT_BROKER_ADDR  "47.102.103.1"
#define MQTT_BROKER_PORT  1883

static emcute_topic_t topic;
static const char *state_msg[] = { "work", "not work" };

int main(void) {
    // Setup the message queue
    msg_t queue[8];
    msg_init_queue(queue, 8);

    // Initialize emcute
    if (emcute_sock == -1) {
        puts("Failed to initialize MQTT-SN client");
        return 1;
    }

    // Connect to the broker
    sock_udp_ep_t gw = { NULL, htons(MQTT_BROKER_PORT), { { 0 } } };
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    // Register the topic
    topic.name = "device/state";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    // Periodic publish loop
    while (1) {
        for (size_t i = 0; i < sizeof(state_msg)/sizeof(state_msg[0]); i++) {
            int res = emcute_pub(&topic, state_msg[i], strlen(state_msg[i]), EMCUTE_QOS_0);
            if (res != EMCUTE_OK) {
                printf("Error: unable to publish data (%d)\n", res);
            } else {
                printf("Published: %s\n", state_msg[i]);
            }
            xtimer_sleep(5);
        }
    }
    return 0;
}