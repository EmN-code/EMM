#include <thread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "mqtt.h"  // MQTT client header from a suitable library
#include "xtimer.h"  // For timing and periodic function execution

#define BROKER_ADDR    "47.102.103.1"
#define BROKER_PORT    1883
#define PUBLISH_TOPIC  "device/status"
#define CLIENT_ID      "esp32_client"
#define STATE_PERIOD   (5U * 1000000U) // 5 seconds in microseconds

static mqtt_client_t mqtt_client;
static char mqtt_client_id[64];
static char mqtt_topic[64];
static mqtt_message_t message;

static void *publisher_thread(void *arg) {
    (void)arg;
    while (1) {
        /* Toggle state message and publish */
        snprintf((char *)message.payload, sizeof(message.payload), "work");
        message.payloadlen = strlen((char *)message.payload);
        mqtt_publish(&mqtt_client, PUBLISH_TOPIC, &message);

        /* Wait for the STATE_PERIOD before publishing again */
        xtimer_usleep(STATE_PERIOD);
    }
    return NULL;
}

int main(void) {
    strcpy(mqtt_client_id, CLIENT_ID);
    snprintf(mqtt_topic, sizeof(mqtt_topic), PUBLISH_TOPIC);

    /* Initialize MQTT client fields */
    mqtt_init(&mqtt_client, mqtt_client_id);

    /* Connect to MQTT broker */
    mqtt_connect(&mqtt_client, BROKER_ADDR, BROKER_PORT);

    /* Set up publisher thread */
    static char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publisher_thread, NULL, "pub_thread");

    return 0;
}