
#include <stdio.h>
#include <string.h>
#include "net/ipv6/uip-ds6.h"
#include "net/mqtt.h"
#include "xtimer.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS MQTT_QOS_0
#define MQTT_RETAIN MQTT_RETAIN_FALSE
#define MQTT_TOPIC_ALIAS 0
#define MQTT_TOPIC_ALIAS_EN MQTT_TOPIC_ALIAS_EN_FALSE
#define MQTT_KEEP_ALIVE 60
#define MQTT_CLEAN_SESSION 1
#define MQTT_MAX_SEGMENT_SIZE 128
#define MQTT_CLIENT_ID "nrf52840_client"
#define MQTT_PUBLISH_INTERVAL 5

static struct mqtt_connection conn;
static uint16_t mid;

void mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    (void)conn;
    (void)data;
    if (event == MQTT_EVENT_CONNECTED) {
        printf("MQTT connected\n");
    } else if (event == MQTT_EVENT_DISCONNECTED) {
        printf("MQTT disconnected\n");
    }
}

void publish_state(void) {
    char payload[10];
    static int state = 0;
    if (state) {
        strcpy(payload, "work");
    } else {
        strcpy(payload, "not work");
    }
    state = !state;

    mqtt_status_t status = mqtt_publish(&conn, &mid, MQTT_TOPIC, (uint8_t *)payload, strlen(payload), MQTT_QOS, MQTT_RETAIN, MQTT_TOPIC_ALIAS, MQTT_TOPIC_ALIAS_EN, NULL);
    if (status == MQTT_STATUS_SUCCESS) {
        printf("Published: %s\n", payload);
    } else {
        printf("Failed to publish\n");
    }
}

int main(void) {
    printf("Starting MQTT client\n");

    mqtt_status_t status = mqtt_register(&conn, NULL, MQTT_CLIENT_ID, mqtt_event_callback, MQTT_MAX_SEGMENT_SIZE);
    if (status != MQTT_STATUS_SUCCESS) {
        printf("Failed to register MQTT client\n");
        return 1;
    }

    status = mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, MQTT_KEEP_ALIVE, MQTT_CLEAN_SESSION, NULL);
    if (status != MQTT_STATUS_SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_sleep(MQTT_PUBLISH_INTERVAL);
    }

    return 0;
}
