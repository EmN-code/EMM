#include <stdio.h>
#include "net/mac/tsch/tsch.h"
#include "mqtt.h"
#include "contiki.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_TOPIC "office/state"
#define CLIENT_ID "nrf52840"

static struct etimer publish_timer;
static struct mqtt_connection conn;
static char publish_payload[20];

PROCESS(mqtt_pub_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_pub_process);

static void mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    switch (event) {
        case MQTT_EVENT_CONNECTED:
            printf("Connected to MQTT broker\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("Disconnected from MQTT broker\n");
            break;
        default:
            printf("MQTT event: %d\n", event);
            break;
    }
}

PROCESS_THREAD(mqtt_pub_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_pub_process, CLIENT_ID, mqtt_event, 512);
    etimer_set(&publish_timer, CLOCK_SECOND * 5);

    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, (3 * CLOCK_SECOND), MQTT_CLEAN_SESSION_ON);

    while(1) {
        PROCESS_YIELD();

        if(ev == PROCESS_EVENT_TIMER && data == &publish_timer) {
            snprintf(publish_payload, sizeof(publish_payload), "Work\n");
            mqtt_publish(&conn, NULL, PUBLISH_TOPIC, (uint8_t *)publish_payload, strlen(publish_payload), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
            printf("Published: %s\n", publish_payload);
            etimer_reset(&publish_timer);
        }
    }

    PROCESS_END();
}