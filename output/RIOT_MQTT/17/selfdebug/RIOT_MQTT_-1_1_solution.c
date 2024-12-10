#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // For sleep
#include <net/emcute.h> // EMCUTE needs for MQTT
#include <net/sock/udp.h>
#include <pthread.h>
#include "xtimer.h"

#define MQTT_BROKER_IP   "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC       "esp32/status"
#define MQTT_PAYLOAD     "work or not work"
#define INTERVAL         (5U * US_PER_SEC) // 5 seconds

static void *_publish_thread(void *arg) {
    (void)arg;

    mqtt_client_t *client = (mqtt_client_t *)malloc(sizeof(mqtt_client_t));
    if (!client) {
        printf("Failed to allocate memory for MQTT client.\n");
        return NULL;
    }

    Network network;
    MQTTClientInit(client, &network, 1000, NULL, 0, NULL, 0);

    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != 0) {
        printf("Failed to connect to the MQTT broker.\n");
        free(client);
        return NULL;
    }

    printf("Connected to MQTT broker at %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);

    while (1) {
        MQTTPublish(client, MQTT_TOPIC, MQTT_PAYLOAD);
        printf("Published %s to topic %s\n", MQTT_PAYLOAD, MQTT_TOPIC);
        xtimer_sleep(INTERVAL);
    }

    NetworkDisconnect(&network);
    free(client);
    return NULL;
}

int main(void) {
    pthread_t thread;
    int result = pthread_create(&thread, NULL, _publish_thread, NULL);
    if (result != 0) {
        printf("Error creating thread: %d\n", result);
        return EXIT_FAILURE;
    }

    pthread_join(thread, NULL);
    return EXIT_SUCCESS;
}