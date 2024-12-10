
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "net/mqtt.h"  // Ensure this path is correct

#define BROKER_HOST "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_SIZE 4

struct mqtt_connection conn;

void publish_state(void) {
    uint16_t mid;
    mqtt_status_t status = mqtt_publish(&conn, &mid, TOPIC, (uint8_t *)PAYLOAD, PAYLOAD_SIZE, MQTT_QOS_LEVEL_0, MQTT_RETAIN_FALSE, 0, MQTT_TOPIC_ALIAS_EN_FALSE, NULL);
    if (status != MQTT_STATUS_SUCCESS) {
        printf("Failed to publish message\n");
    } else {
        printf("Message published\n");
    }
}

int main(void) {
    // Initialize the MQTT connection
    mqtt_status_t status = mqtt_connect(&conn, BROKER_HOST, BROKER_PORT, 60, MQTT_CLEAN_SESSION_TRUE, NULL);
    if (status != MQTT_STATUS_SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    printf("Connected to MQTT broker\n");

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        sleep(5);
    }

    // Disconnect from the MQTT broker
    mqtt_disconnect(&conn, NULL);

    return 0;
}
