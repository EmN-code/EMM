
#include <stdio.h>
#include <string.h>
#include "net/mqtt.h"
#include "xtimer.h"

#define MQTT_BROKER_HOST "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_CLIENT_ID "nrf52840_client"
#define MQTT_KEEP_ALIVE 60
#define MQTT_QOS MQTT_QOS_0

static struct mqtt_connection conn;
static uint16_t mid;

void mqtt_event_handler(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    if (event == MQTT_EVENT_CONNECT) {
        printf("MQTT connected\n");
    } else if (event == MQTT_EVENT_DISCONNECT) {
        printf("MQTT disconnected\n");
    }
}

void publish_state(void) {
    mqtt_status_t status = mqtt_publish(&conn, &mid, MQTT_TOPIC, (uint8_t *)MQTT_PAYLOAD, strlen(MQTT_PAYLOAD), MQTT_QOS, MQTT_RETAIN_FALSE, 0, MQTT_TOPIC_ALIAS_EN_FALSE, NULL);
    if (status != MQTT_STATUS_SUCCESS) {
        printf("Failed to publish message\n");
    } else {
        printf("Published: %s\n", MQTT_PAYLOAD);
    }
}

int main(void) {
    mqtt_status_t status;

    status = mqtt_register(&conn, NULL, MQTT_CLIENT_ID, mqtt_event_handler, 0);
    if (status != MQTT_STATUS_SUCCESS) {
        printf("Failed to register MQTT client\n");
        return 1;
    }

    status = mqtt_connect(&conn, MQTT_BROKER_HOST, MQTT_BROKER_PORT, MQTT_KEEP_ALIVE, MQTT_CLEAN_SESSION_TRUE, NULL);
    if (status != MQTT_STATUS_SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
