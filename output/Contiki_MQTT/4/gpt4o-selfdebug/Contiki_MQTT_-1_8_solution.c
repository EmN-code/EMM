#include "mqtt.h"
#include "contiki.h"
#include "sys/etimer.h"

#include <stdio.h>
#include <string.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define KEEP_ALIVE 60
#define CLIENT_ID "nrf52840_client"
#define TOPIC "device/state"

static struct mqtt_connection conn;
static struct etimer timer;

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

static void mqtt_event_callback(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    if (event == MQTT_EVENT_CONNECTED) {
        printf("Connected to MQTT broker\n");
    } else if (event == MQTT_EVENT_PUBLISHED) {
        printf("Message published\n");
    }
}

PROCESS_THREAD(mqtt_example_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_example_process, CLIENT_ID, mqtt_event_callback, MQTT_MAX_INPUT_BUFFER_SIZE);
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, KEEP_ALIVE);

    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_YIELD();
        if (etimer_expired(&timer)) {
            mqtt_publish(&conn, NULL, TOPIC, (uint8_t *)"work", strlen("work"), MQTT_QOS_LEVEL_0, MQTT_ARRANGE_NOT_RETAINED, MQTT_TOPIC_ALIAS_DISABLED);
            etimer_reset(&timer);
        }
    }

    PROCESS_END();
}
