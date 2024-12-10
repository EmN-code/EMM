#include <stdio.h>
#include <stdlib.h>
#include "mqtt.h"
#include "net/netstack.h"

/* Configuration */
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID "nrf52840_device"
#define PUBLISH_TOPIC "device/state"
#define PUBLISH_INTERVAL 5
#define MAX_SEGMENT_SIZE 128

/* MQTT Variables */
static struct mqtt_connection conn;
process_event_t mqtt_event;

/* Function prototypes */
static void mqtt_event_handler(struct mqtt_connection *m, mqtt_event_t ev, void *data);

/* MQTT Event Handler */
static void mqtt_event_handler(struct mqtt_connection *m, mqtt_event_t ev, void *data) {
    switch(ev) {
        case MQTT_DATA:
            /* Handle incoming data */
            break;
        case MQTT_EVENT_CONNECTED:
            /* Handle successful connection */
            break;
        case MQTT_EVENT_DISCONNECTED:
            /* Handle disconnection */
            break;
        default:
            break;
    }
}

/* Main function */
int main(void) {
    printf("Starting MQTT Client\n");

    // Register MQTT process
    mqtt_register(&conn, this_process, CLIENT_ID, mqtt_event_handler, MAX_SEGMENT_SIZE);

    // Set MQTT broker address
    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    while (1) {
        // Publish message every PUBLISH_INTERVAL seconds
        char *message = "work";
        mqtt_publish(&conn, NULL, PUBLISH_TOPIC, (uint8_t *)message, strlen(message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
        sleep(PUBLISH_INTERVAL);
    }
    return 0;
}