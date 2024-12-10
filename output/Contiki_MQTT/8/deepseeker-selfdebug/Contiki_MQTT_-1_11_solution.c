
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "mqtt.h"
#include "net/sock/tcp.h"

#define BROKER_HOST "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_SIZE 4
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static struct mqtt_connection conn;
static uint16_t mid;

void mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    (void)conn;
    (void)data;
    switch (event) {
        case MQTT_EVENT_CONNECT:
            printf("Connected to MQTT broker\n");
            break;
        case MQTT_EVENT_DISCONNECT:
            printf("Disconnected from MQTT broker\n");
            break;
        case MQTT_EVENT_PUBLISH:
            printf("Message published\n");
            break;
        default:
            break;
    }
}

int main(void) {
    printf("Starting MQTT client\n");

    // Initialize MQTT connection
    mqtt_status_t status = mqtt_register(&conn, NULL, "nrf52840_client", mqtt_event_callback, 0);
    if (status != MQTT_OK) {
        printf("MQTT registration failed\n");
        return 1;
    }

    // Connect to MQTT broker
    status = mqtt_connect(&conn, BROKER_HOST, BROKER_PORT, 60, 1, NULL);
    if (status != MQTT_OK) {
        printf("MQTT connection failed\n");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        status = mqtt_publish(&conn, &mid, TOPIC, (uint8_t *)PAYLOAD, PAYLOAD_SIZE, MQTT_QOS_0, MQTT_RETAIN_FALSE, 0, MQTT_TOPIC_ALIAS_EN_FALSE, NULL);
        if (status != MQTT_OK) {
            printf("MQTT publish failed\n");
        }
        xtimer_usleep(INTERVAL);
    }

    // Disconnect from MQTT broker
    mqtt_disconnect(&conn, NULL);

    return 0;
}
