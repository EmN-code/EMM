
#include <stdio.h>
#include <string.h>
#include "net/mqtt.h"
#include "xtimer.h"

#define MQTT_BROKER_HOST "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "nrf52840_client"
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_KEEP_ALIVE 60

static struct mqtt_connection conn;

void mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    if (event == MQTT_EVENT_CONNECTED) {
        printf("MQTT connected\n");
    } else if (event == MQTT_EVENT_DISCONNECTED) {
        printf("MQTT disconnected\n");
    }
}

int main(void) {
    mqtt_status_t status;
    uint16_t mid;

    // Initialize the MQTT connection
    status = mqtt_register(&conn, NULL, MQTT_CLIENT_ID, mqtt_event_callback, 0);
    if (status != MQTT_STATUS_SUCCESS) {
        printf("MQTT register failed\n");
        return -1;
    }

    // Connect to the MQTT broker
    status = mqtt_connect(&conn, MQTT_BROKER_HOST, MQTT_BROKER_PORT, MQTT_KEEP_ALIVE, 1, NULL);
    if (status != MQTT_STATUS_SUCCESS) {
        printf("MQTT connect failed\n");
        return -1;
    }

    while (1) {
        // Publish the state
        status = mqtt_publish(&conn, &mid, MQTT_TOPIC, (uint8_t *)MQTT_PAYLOAD, strlen(MQTT_PAYLOAD), MQTT_QOS_0, 0, 0, 0, NULL);
        if (status != MQTT_STATUS_SUCCESS) {
            printf("MQTT publish failed\n");
        } else {
            printf("MQTT publish success\n");
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    // Disconnect from the MQTT broker
    mqtt_disconnect(&conn, NULL);

    return 0;
}
