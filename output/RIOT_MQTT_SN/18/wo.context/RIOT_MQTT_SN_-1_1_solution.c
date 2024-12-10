#include <stdio.h>
#include <string.h>
#include "emcute.h"
#include "event.h"
#include "xtimer.h"

#define MQTT_SN_BROKER_IP "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * 1000000U) // 5 seconds
#define DEVICE_STATE "work"

static char stack[THREAD_STACKSIZE_MAIN];
static emcute_topic_t topic;
static char message[64];

// Callback function for EMCUTE publish
void on_message(const emcute_topic_t *topic, void *data, size_t len) {
    printf("Published to topic %s: %.*s\n", topic->name, (int)len, (char *)data);
}

// Function to connect to MQTT-SN broker
static int mqttsn_connect(void) {
    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = MQTT_SN_BROKER_PORT
    };
    inet_pton(AF_INET, MQTT_SN_BROKER_IP, &gw.addr.ipv4);

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("error: unable to connect [%d]\n", res);
        return 1;
    }

    return 0;
}

// Function to publish state
static void *mqttsn_publisher(void *arg) {
    (void) arg;
    topic.name = "state/topic";

    while (1) {
        strncpy(message, DEVICE_STATE, sizeof(message));

        emcute_cb_t cb = (emcute_cb_t) on_message;
        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("error: unable to publish state\n");
        }
        else {
            cb(&topic, message, strlen(message));
        }
        xtimer_sleep(PUBLISH_INTERVAL / 1000000U);
    }

    return NULL;
}

int main(void) {
    event_t init_done = EVENT_INIT;

    if (mqttsn_connect() != 0) {
        printf("MQTT-SN connection failed\n");
        return 1;
    }

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqttsn_publisher, NULL, "mqttsn_pub");

    event_wait(&init_done);

    return 0;
}