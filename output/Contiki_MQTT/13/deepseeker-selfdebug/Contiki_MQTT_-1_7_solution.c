
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "mqtt.h"
#include "net/netstack.h"
#include "net/ipv6/addr.h"

#define MQTT_BROKER_HOST "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static struct mqtt_connection conn;
static char client_id[] = "nrf52840_client";

static void mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    (void)conn;
    (void)data;
    switch (event) {
        case MQTT_EVENT_CONNECTED:
            printf("MQTT connected\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("MQTT disconnected\n");
            break;
        default:
            break;
    }
}

static void publish_state(void) {
    mqtt_status_t status = mqtt_publish(&conn, MQTT_TOPIC, MQTT_MESSAGE, strlen(MQTT_MESSAGE), MQTT_QOS_0, false);
    if (status != MQTT_STATUS_SUCCESS) {
        printf("Failed to publish message\n");
    } else {
        printf("Published: %s\n", MQTT_MESSAGE);
    }
}

int main(void) {
    // Initialize the network stack
    netstack_init();

    // Initialize the MQTT connection
    mqtt_status_t status = mqtt_register(&conn, NULL, client_id, mqtt_event_callback, 0);
    if (status != MQTT_STATUS_SUCCESS) {
        printf("Failed to register MQTT client\n");
        return 1;
    }

    // Connect to the MQTT broker
    status = mqtt_connect(&conn, MQTT_BROKER_HOST, MQTT_BROKER_PORT, 60, 1, NULL);
    if (status != MQTT_STATUS_SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(MQTT_INTERVAL);
    }

    return 0;
}
