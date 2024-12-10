
#include <stdio.h>
#include <string.h>
#include "net/mqtt.h"
#include "net/netif.h"
#include "net/ipv6/addr.h"
#include "xtimer.h"

#define MQTT_BROKER_HOST "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_SIZE 4
#define MQTT_KEEP_ALIVE 60
#define MQTT_QOS MQTT_QOS_0

static struct mqtt_connection conn;

void publish_state(void) {
    uint16_t mid;
    mqtt_status_t status = mqtt_publish(&conn, &mid, MQTT_TOPIC, (uint8_t *)MQTT_PAYLOAD, MQTT_PAYLOAD_SIZE, MQTT_QOS, MQTT_RETAIN_FALSE, 0, MQTT_TOPIC_ALIAS_EN_FALSE, NULL);
    if (status != MQTT_OK) {
        printf("Failed to publish message: %d\n", status);
    } else {
        printf("Published message: %s\n", MQTT_PAYLOAD);
    }
}

int main(void) {
    printf("Starting MQTT client...\n");

    // Initialize the MQTT connection
    mqtt_init(&conn);

    // Connect to the MQTT broker
    mqtt_status_t status = mqtt_connect(&conn, MQTT_BROKER_HOST, MQTT_BROKER_PORT, MQTT_KEEP_ALIVE, MQTT_CLEAN_SESSION_TRUE, NULL);
    if (status != MQTT_OK) {
        printf("Failed to connect to MQTT broker: %d\n", status);
        return 1;
    }

    printf("Connected to MQTT broker at %s:%d\n", MQTT_BROKER_HOST, MQTT_BROKER_PORT);

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    // Disconnect from the MQTT broker
    mqtt_disconnect(&conn);

    return 0;
}
