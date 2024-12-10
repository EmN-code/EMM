#include "contiki.h"
#include "mqtt.h"
#include <stdio.h>

#define BROKER_IP_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "nrf52840_mqtt_client"
#define MQTT_TOPIC "state/topic"
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND) // 5 seconds
#define MAX_PAYLOAD_LENGTH 64
#define MQTT_MAX_SEGMENT_SIZE 128
#define MQTT_KEEP_ALIVE_DEFAULT 60

PROCESS(mqtt_client_process, "MQTT Client Process");
AUTOSTART_PROCESSES(&mqtt_client_process);

static struct mqtt_connection conn;
static char pub_state_msg[MAX_PAYLOAD_LENGTH];

static void mqtt_event_cb(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    switch (event) {
        case MQTT_EVENT_CONNECTED:
            printf("Connected to the broker\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("Disconnected from the broker\n");
            break;
        case MQTT_EVENT_PUBLISH:
            printf("Message published successfully\n");
            break;
        default:
            printf("Got unhandled MQTT event: %d\n", event);
            break;
    }
}

PROCESS_THREAD(mqtt_client_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_client_process, CLIENT_ID, mqtt_event_cb, MQTT_MAX_SEGMENT_SIZE);
    mqtt_connect(&conn, BROKER_IP_ADDR, BROKER_PORT, MQTT_KEEP_ALIVE_DEFAULT);

    static struct etimer periodic_timer;
    etimer_set(&periodic_timer, PUBLISH_INTERVAL);

    while(1) {
        PROCESS_YIELD();

        if(etimer_expired(&periodic_timer)) {
            snprintf(pub_state_msg, sizeof(pub_state_msg), "%s", "work or not work");
            mqtt_publish(&conn, NULL, MQTT_TOPIC, (uint8_t *)pub_state_msg, strlen(pub_state_msg), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
            printf("Published: %s\n", pub_state_msg);

            etimer_set(&periodic_timer, PUBLISH_INTERVAL); // reset the timer
        }
    }

    PROCESS_END();
}