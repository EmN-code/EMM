
#include <stdio.h>
#include <string.h>
#include "net/mqtt.h"
#include "xtimer.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_SIZE 4

static struct mqtt_connection conn;

void publish_state(void) {
    uint16_t mid;
    mqtt_status_t status = mqtt_publish(&conn, &mid, TOPIC, (uint8_t *)PAYLOAD, PAYLOAD_SIZE, MQTT_QOS_LEVEL_0, MQTT_RETAIN_FALSE, 0, MQTT_TOPIC_ALIAS_EN_FALSE, NULL);
    if (status != MQTT_STATUS_SUCCESS) {
        printf("Failed to publish state\n");
    } else {
        printf("State published\n");
    }
}

int main(void) {
    printf("Connecting to MQTT broker...\n");
    mqtt_status_t status = mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 60, MQTT_CLEAN_SESSION_TRUE, NULL);
    if (status != MQTT_STATUS_SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    printf("Connected to MQTT broker\n");

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    mqtt_disconnect(&conn, NULL);
    return 0;
}
