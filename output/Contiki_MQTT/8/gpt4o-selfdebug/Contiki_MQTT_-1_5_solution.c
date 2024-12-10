#include <stdio.h>
#include "mqtt.h"  // Correct MQTT header
#include "contiki.h"

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

static struct mqtt_connection conn;
static char client_id[20];
static char message[15];
static struct etimer timer;

PROCESS_THREAD(mqtt_publish_process, ev, data) {
    PROCESS_BEGIN();

    snprintf(client_id, sizeof(client_id), "nrf52840");
    mqtt_register(&conn, &mqtt_publish_process, client_id, NULL, MAX_TCP_SEGMENT_SIZE);

    mqtt_connect(&conn, "47.102.103.1", 1883, 60);

    while (1) {
        etimer_set(&timer, CLOCK_SECOND * 5);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        snprintf(message, sizeof(message), "%s", "work"); // Preparing the state message

        mqtt_publish(&conn, NULL, "device/state", (uint8_t *)message, strlen(message), MQTT_QOS_LEVEL_0, 0, 0, 0);
    }

    PROCESS_END();
}