#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "net/emcute.h"
#include "msg.h"
#include "thread.h"

#define MQTT_BROKER_ADDRESS  "47.102.103.1"
#define MQTT_BROKER_PORT     (1883)
#define MQTT_TOPIC_STATE     "device/state"
#define MQTT_STATE_WORK      "work"
#define MQTT_STATE_NOT_WORK  "not work"
#define PUBLISH_INTERVAL     (5U)  // In seconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];
static emcute_topic_t t;

static void *mqtt_thread(void *arg) {
    (void)arg;
    char *state = MQTT_STATE_WORK;
    uint8_t flags = EMCUTE_QOS_0;
    int res;

    while (1) {
        res = emcute_pub(&t, state, strlen(state), flags);
        if (res != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        }
        xtimer_sleep(PUBLISH_INTERVAL);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, 8);

    // Set up the address structure
    sock_udp_ep_t gw = {
        .family = AF_INET6,
        .port = MQTT_BROKER_PORT
    };
    inet_pton(AF_INET, MQTT_BROKER_ADDRESS, &gw.addr.ipv6);

    // Connect to the broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Unable to connect to MQTT-SN broker\n");
        return 1;
    }

    // Set up the topic
    t.name = MQTT_TOPIC_STATE;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("Unable to register topic\n");
        return 1;
    }

    // Start the MQTT thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  mqtt_thread, NULL, "mqtt_thread");

    // Keep the main thread alive
    while (1) {
        xtimer_sleep(60);
    }
    return 0;
}