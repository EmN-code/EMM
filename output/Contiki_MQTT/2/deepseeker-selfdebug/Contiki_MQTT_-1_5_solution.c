
#include <stdio.h>
#include <string.h>
#include "net/mqtt.h"
#include "xtimer.h"

#define BROKER_HOST "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_SIZE 4
#define INTERVAL 5

static struct mqtt_connection conn;

void publish_state(void) {
    uint16_t mid;
    mqtt_status_t status = mqtt_publish(&conn, &mid, TOPIC, (uint8_t *)PAYLOAD, PAYLOAD_SIZE, MQTT_QOS_0, MQTT_RETAIN_FALSE, 0, MQTT_TOPIC_ALIAS_EN_FALSE, NULL);
    if (status != MQTT_OK) {
        printf("Failed to publish message: %d\n", status);
    } else {
        printf("Published message: %s\n", PAYLOAD);
    }
}

int main(void) {
    mqtt_status_t status;

    // Initialize MQTT connection
    memset(&conn, 0, sizeof(conn));

    // Connect to MQTT broker
    status = mqtt_connect(&conn, BROKER_HOST, BROKER_PORT, 60, MQTT_CLEAN_SESSION_TRUE, NULL);
    if (status != MQTT_OK) {
        printf("Failed to connect to MQTT broker: %d\n", status);
        return 1;
    }

    printf("Connected to MQTT broker\n");

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_sleep(INTERVAL);
    }

    // Disconnect from MQTT broker
    mqtt_disconnect(&conn, NULL);

    return 0;
}
