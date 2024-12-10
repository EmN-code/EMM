
#include <stdio.h>
#include <string.h>
#include "net/mqtt.h"
#include "xtimer.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_SIZE 4
#define INTERVAL 5

static struct mqtt_connection conn;

void event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    if (event == MQTT_EVENT_CONNECT) {
        printf("Connected to MQTT broker\n");
    } else if (event == MQTT_EVENT_DISCONNECT) {
        printf("Disconnected from MQTT broker\n");
    }
}

int main(void) {
    mqtt_status_t status;
    uint16_t mid;

    status = mqtt_register(&conn, NULL, "nrf52840_client", event_callback, 0);
    if (status != MQTT_STATUS_SUCCESS) {
        printf("Failed to register MQTT client\n");
        return 1;
    }

    status = mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 60, 1, NULL);
    if (status != MQTT_STATUS_SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    while (1) {
        status = mqtt_publish(&conn, &mid, TOPIC, (uint8_t *)PAYLOAD, PAYLOAD_SIZE, MQTT_QOS_0, MQTT_RETAIN_FALSE, 0, MQTT_TOPIC_ALIAS_EN_FALSE, NULL);
        if (status != MQTT_STATUS_SUCCESS) {
            printf("Failed to publish message\n");
        } else {
            printf("Published message: %s\n", PAYLOAD);
        }

        xtimer_sleep(INTERVAL);
    }

    mqtt_disconnect(&conn, NULL);
    return 0;
}
