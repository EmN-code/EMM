#include <stdio.h>
#include "mqtt.h"
#include "contiki.h"
#include "process.h"
#include "sys/etimer.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883

static struct mqtt_connection conn;
static struct etimer periodic_timer;

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

PROCESS_THREAD(mqtt_example_process, ev, data) {
    PROCESS_BEGIN();

    etimer_set(&periodic_timer, CLOCK_SECOND * 5);

    mqtt_register(&conn, &mqtt_example_process, "nrf52840_client", NULL, MQTT_MAX_SEGMENT_SIZE);

    // Establish connection to the MQTT broker
    if (mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 100) != MQTT_STATUS_OK) {
        printf("Failed to connect to MQTT broker\n");
        PROCESS_EXIT();
    }

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

        // Publish "work" state to topic "state"
        mqtt_publish(&conn, NULL, "state", (uint8_t *)"work", 4, MQTT_QOS_LEVEL_0, MQTT_5);
        printf("Published state: work\n");

        etimer_reset(&periodic_timer);
    }

    PROCESS_END();
}