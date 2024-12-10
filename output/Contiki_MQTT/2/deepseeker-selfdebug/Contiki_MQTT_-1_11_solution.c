
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/mqtt.h"
#include "net/netdev.h"
#include "net/netdev/eth.h"
#include "net/netif.h"
#include "net/sock/tcp.h"
#include "net/sock/util.h"
#include "periph/gpio.h"
#include "shell.h"

#define MQTT_BROKER_HOST "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "nrf52840_client"
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_KEEP_ALIVE 60

static struct mqtt_connection conn;

static void mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data)
{
    (void)conn;
    (void)data;

    switch (event) {
        case MQTT_EVENT_CONNECT:
            printf("MQTT connected\n");
            break;
        case MQTT_EVENT_DISCONNECT:
            printf("MQTT disconnected\n");
            break;
        case MQTT_EVENT_PUBLISH:
            printf("MQTT message published\n");
            break;
        default:
            break;
    }
}

static void publish_state(void)
{
    uint16_t mid;
    mqtt_status_t status = mqtt_publish(&conn, &mid, MQTT_TOPIC, (uint8_t *)MQTT_PAYLOAD, strlen(MQTT_PAYLOAD), MQTT_QOS_0, MQTT_RETAIN_FALSE, 0, MQTT_TOPIC_ALIAS_EN_FALSE, NULL);
    if (status != MQTT_STATUS_SUCCESS) {
        printf("Failed to publish message\n");
    }
}

int main(void)
{
    printf("Starting MQTT client\n");

    // Initialize MQTT connection
    mqtt_status_t status = mqtt_register(&conn, NULL, MQTT_CLIENT_ID, mqtt_event_callback, 0);
    if (status != MQTT_STATUS_SUCCESS) {
        printf("Failed to register MQTT client\n");
        return 1;
    }

    status = mqtt_connect(&conn, MQTT_BROKER_HOST, MQTT_BROKER_PORT, MQTT_KEEP_ALIVE, MQTT_CLEAN_SESSION_TRUE, NULL);
    if (status != MQTT_STATUS_SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
