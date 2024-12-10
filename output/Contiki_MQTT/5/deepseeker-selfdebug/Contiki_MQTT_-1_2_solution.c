
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/mqtt.h"
#include "net/ipv6/addr.h"
#include "net/sock/tcp.h"

#define MQTT_BROKER_HOST "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS MQTT_QOS_0
#define MQTT_RETAIN MQTT_RETAIN_FALSE
#define MQTT_KEEP_ALIVE 60
#define MQTT_CLEAN_SESSION 1

static struct mqtt_connection conn;
static uint16_t mid;

void mqtt_event_callback(struct mqtt_connection *connection, mqtt_event_t event, void *data) {
    (void)connection;
    (void)data;
    if (event == MQTT_EVENT_CONNECT) {
        printf("MQTT connected\n");
    } else if (event == MQTT_EVENT_DISCONNECT) {
        printf("MQTT disconnected\n");
    }
}

int main(void) {
    // Initialize MQTT connection
    mqtt_register(&conn, NULL, "nrf52840_client", mqtt_event_callback, 0);

    // Connect to MQTT broker
    mqtt_status_t status = mqtt_connect(&conn, MQTT_BROKER_HOST, MQTT_BROKER_PORT, MQTT_KEEP_ALIVE, MQTT_CLEAN_SESSION, NULL);
    if (status != MQTT_OK) {
        printf("MQTT connection failed\n");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        status = mqtt_publish(&conn, &mid, MQTT_TOPIC, (uint8_t *)MQTT_PAYLOAD, strlen(MQTT_PAYLOAD), MQTT_QOS, MQTT_RETAIN, 0, MQTT_TOPIC_ALIAS_EN_FALSE, NULL);
        if (status != MQTT_OK) {
            printf("MQTT publish failed\n");
        } else {
            printf("MQTT publish successful\n");
        }
        xtimer_sleep(5);
    }

    // Disconnect from MQTT broker
    mqtt_disconnect(&conn, NULL);

    return 0;
}
