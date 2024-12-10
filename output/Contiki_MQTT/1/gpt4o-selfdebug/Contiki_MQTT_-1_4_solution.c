#include "sensors.h"
#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "mqtt.h"
#include "contiki.h"
#include "timers.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL 5

static struct mqtt_connection conn;
static char *state_topic = "device/state";

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

static void mqtt_event_callback(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    printf("MQTT event: %d\n", event);
    if (event == MQTT_EVENT_CONNECTED) {
        printf("Connected to the broker\n");
    } else if (event == MQTT_EVENT_DISCONNECTED) {
        printf("Disconnected from the broker\n");
    }
}

PROCESS_THREAD(mqtt_publish_process, ev, data) {
    static struct etimer timer;
    static char payload[20];
    static uint16_t packet_id;
    
    PROCESS_BEGIN();
    
    mqtt_register(&conn, &mqtt_publish_process, "nrf52840_client", mqtt_event_callback, 128);
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 60);
    
    etimer_set(&timer, CLOCK_SECOND * PUBLISH_INTERVAL);
    
    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        snprintf(payload, sizeof(payload), "work");
        mqtt_publish(&conn, &packet_id, state_topic, (uint8_t *)payload, strlen(payload), MQTT_QOS_LEVEL_0);

        etimer_reset(&timer);
    }
    
    PROCESS_END();
}