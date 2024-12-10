#include "mqtt.h"
#include "contiki.h"
#include <stdint.h>
#include <stdio.h>

#define MQTT_BUF_SIZE 256
#define MQTT_MID_SIZE 1

PROCESS(mqtt_publisher_process, "MQTT Publisher");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

static struct mqtt_connection conn;
static char client_id[23] = "mqtt_nrf52840_client";
static char pub_topic[] = "device/status";
static uint8_t pub_flags = MQTT_QOS_LEVEL_0;
static char mqtt_buffer[MQTT_BUF_SIZE];
static char mqtt_client_id[MQTT_MID_SIZE];

static int is_working = 1;

PROCESS_THREAD(mqtt_publisher_process, ev, data)
{
    static struct etimer timer;
    static uint16_t message_id;

    PROCESS_BEGIN();

    printf("Starting MQTT Publisher\n");

    mqtt_register(&conn, &mqtt_publisher_process, client_id, mqtt_event, sizeof(mqtt_buffer), mqtt_buffer);

    mqtt_connect(&conn, "47.102.103.1", 1883, (60 * CLOCK_SECOND) / CLOCK_SECOND);

    while(1) {
        etimer_set(&timer, 5 * CLOCK_SECOND);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        char* state = is_working ? "work" : "not work";
        snprintf(mqtt_buffer, sizeof(mqtt_buffer), "{\"state\": \"%s\"}", state);
        mqtt_publish(&conn, &message_id, pub_topic, (uint8_t *)mqtt_buffer, strlen(mqtt_buffer), MQTT_QOS_LEVEL_0);
    }

    PROCESS_END();
}

void mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    switch (event) {
        case MQTT_EVENT_CONNECTED:
            printf("Connected to MQTT broker\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("Disconnected from MQTT broker\n");
            break;
        default:
            break;
    }
}