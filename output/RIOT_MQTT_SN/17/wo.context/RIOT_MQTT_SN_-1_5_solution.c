#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "net/sock/udp.h"
#include "net/sock/tcp.h"
#include "net/af.h"
#include "xtimer.h"
#include "net/emcute.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TOPIC_MAXLEN 32
#define BUFFER_SIZE 64
#define TOPIC "device/state"
#define TIME_INTERVAL 5

static char mqtt_topic[TOPIC_MAXLEN];
static emcute_topic_t t;
xtimer_ticks32_t last_wakeup;

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    /* Callback for receiving published data, not used in this example */
    (void)topic;
    (void)data;
    (void)len;
}

int main(void) {
    int res;
    sock_udp_ep_t gw = { .port = MQTT_BROKER_PORT };

    // Convert the IP address to binary form and store it in gw
    res = inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);
    if (res == 0) {
        puts("ERROR: Unable to parse MQTT broker IP address.");
        return 1;
    }

    // Initialize the EMCUTE client
    res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        puts("ERROR: Unable to connect to MQTT-SN broker.");
        return 1;
    }

    // Register a topic
    strcpy(mqtt_topic, TOPIC);
    t.name = mqtt_topic;
    res = emcute_reg(&t);
    if (res != EMCUTE_OK) {
        puts("ERROR: Unable to register topic.");
        return 1;
    }

    puts("Connected to MQTT-SN broker and registered topic.");

    while (1) {
        last_wakeup = xtimer_now();

        const char *state = "work"; // Publish "work"
        printf("Publishing state: %s\n", state);

        res = emcute_pub(&t, state, strlen(state), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            puts("ERROR: Unable to publish data.");
        }

        // Sleep for TIME_INTERVAL seconds
        xtimer_sleep(TIME_INTERVAL);
    }

    return 0;
}