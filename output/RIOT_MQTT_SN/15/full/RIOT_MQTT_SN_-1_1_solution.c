#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "xtimer.h"
#include "net/emcute.h"

#define MQTT_TOPIC        "riot/test"
#define MQTT_BROKER_IP    "47.102.103.1"
#define MQTT_BROKER_PORT  1883
#define MQTT_PUBLISH_MSG  "work"

static emcute_topic_t topic;

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    printf("Received message: %.*s\n", (int)len, (char *)data);
}

int main(void) {
    emcute_sub_t subscription;
    subscription.topic = &topic;
    subscription.cb = on_pub;

    emcute_con_t client;

    int rc;

    // Initialize Emcute
    rc = emcute_init(&subscription, 1);
    if (rc != 0) {
        printf("Failed to initialize emcute\n");
        return 1;
    }

    while (1) {
        // Connect
        rc = emcute_con(&client, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if(rc != 0){
            printf("Failed to connect to broker\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Connected to broker\n");
            break;
        }
    }

    // Prepare the topic
    topic.name = MQTT_TOPIC;

    while (1) {
        // Publish "work" message every 5 seconds
        if (emcute_pub(&topic, MQTT_PUBLISH_MSG, strlen(MQTT_PUBLISH_MSG), EMCUTE_QOS_0) != 0) {
            printf("Failed to publish message\n");
        } else {
            printf("Published message: %s\n", MQTT_PUBLISH_MSG);
        }

        xtimer_sleep(5);
    }

    return 0;
}