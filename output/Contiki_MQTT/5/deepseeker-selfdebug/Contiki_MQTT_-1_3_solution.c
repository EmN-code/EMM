
#include <stdio.h>
#include <string.h>
#include "net/mqtt.h"
#include "net/netif.h"
#include "xtimer.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define MESSAGE "work"
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static struct mqtt_connection conn;

void publish_state(void) {
    uint16_t mid;
    mqtt_status_t status = mqtt_publish(&conn, &mid, TOPIC, (uint8_t *)MESSAGE, strlen(MESSAGE), MQTT_QOS_0, MQTT_RETAIN_FALSE, 0, MQTT_TOPIC_ALIAS_EN_FALSE, NULL);
    if (status != MQTT_OK) {
        printf("Failed to publish message: %d\n", status);
    } else {
        printf("Published message: %s\n", MESSAGE);
    }
}

int main(void) {
    // Initialize the MQTT connection
    mqtt_status_t status = mqtt_register(&conn, NULL, "nrf52840_client", NULL, 0);
    if (status != MQTT_OK) {
        printf("Failed to register MQTT client: %d\n", status);
        return 1;
    }

    // Connect to the MQTT broker
    status = mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 60, MQTT_CLEAN_SESSION_TRUE, NULL);
    if (status != MQTT_OK) {
        printf("Failed to connect to MQTT broker: %d\n", status);
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    // Disconnect from the MQTT broker
    mqtt_disconnect(&conn, NULL);

    return 0;
}
