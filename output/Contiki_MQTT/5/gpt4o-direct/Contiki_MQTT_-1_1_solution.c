#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "mqtt.h"
#include "net.h"
#include "kernel_init.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define DEVICE_ID "nrf52840"
#define TOPIC "device/state"
#define PUB_INTERVAL 5

static mqtt_client_t mqtt_client;
static char mqtt_client_id[64];
static mqtt_publish_data_t pub_data;

static void on_con_event(const mqtt_client_t *client, mqtt_connection_event_t event);

static void on_con_event(const mqtt_client_t *client, mqtt_connection_event_t event) {
    if (event == MQTT_CONNECTED) {
        printf("Connected to the MQTT broker at %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);
    } else if (event == MQTT_DISCONNECTED) {
        printf("Disconnected from the MQTT broker\n");
    }
}

int main(void) {
    kernel_init();
    sprintf(mqtt_client_id, "%s", DEVICE_ID);

    mqtt_client_init(&mqtt_client, MQTT_BROKER_IP, MQTT_BROKER_PORT, 0, mqtt_client_id, NULL, NULL);
    mqtt_register_callback(&mqtt_client, on_con_event);

    if (mqtt_connect(&mqtt_client) != MQTT_OK) {
        printf("Failed to connect to the MQTT broker\n");
        return EXIT_FAILURE;
    }

    while (1) {
        pub_data.payload = ((rand() % 2) == 0) ? "work" : "not work";
        pub_data.payload_len = strlen(pub_data.payload);
        pub_data.qos = 0;

        if (mqtt_publish(&mqtt_client, TOPIC, &pub_data) != MQTT_OK) {
            printf("Publishing failed\n");
        } else {
            printf("Published state: %s\n", pub_data.payload);
        }

        sleep(PUB_INTERVAL);
    }

    return EXIT_SUCCESS;
}