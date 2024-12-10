#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/emcute.h>
#include <net/conn.h>
#include <net.h>
#include <errno.h>
#include "shell.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state/status"
#define MQTT_CLIENT_ID "esp32_client"
#define PUB_INTERVAL (5U)

static const char *states[] = {"work", "not work"};

static void *mqtt_publish_thread(void *arg) {
    emcute_sock_t sock;
    emcute_topic_t topic;
    (void)arg;

    memset(&sock, 0, sizeof(sock));
    if (emcute_connect(&sock, MQTT_BROKER_IP, MQTT_BROKER_PORT, MQTT_CLIENT_ID) != 0) {
        printf("Failed to connect to MQTT broker.\n");
        return NULL;
    }

    topic.name = MQTT_TOPIC;

    while (1) {
        static int state_index = 0;
        char *state = (char *)states[state_index];

        if (emcute_publish(&sock, &topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Failed to publish message.\n");
        } else {
            printf("Published: %s\n", state);
        }

        state_index ^= 1;  // Toggle between 0 and 1

        xtimer_sleep(PUB_INTERVAL);
    }

    emcute_disconnect(&sock);
    return NULL;
}

int main(void) {
    char mqtt_stack[THREAD_STACKSIZE_MAIN];

    /* Start the MQTT publish thread */
    thread_create(mqtt_stack, sizeof(mqtt_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_publish_thread, NULL, "mqtt_pub");

    /* The main thread just sleeps and lets the MQTT thread run */
    while (1) {
        xtimer_sleep(3600);
    }

    return 0;
}