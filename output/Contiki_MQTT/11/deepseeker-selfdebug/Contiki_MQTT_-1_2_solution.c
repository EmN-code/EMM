
#include <stdio.h>
#include <string.h>
#include "contiki.h"
#include "mqtt.h"
#include "mqtt-transport.h" // Ensure this header is included

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"

static struct mqtt_connection conn;
static char client_id[] = "nrf52840_client";
static char message[] = "work";

static void event_callback(struct mqtt_connection *c, mqtt_event_t event, void *data) {
    if (event == MQTT_EVENT_CONNECTED) {
        printf("Connected to MQTT broker\n");
    } else if (event == MQTT_EVENT_DISCONNECTED) {
        printf("Disconnected from MQTT broker\n");
    }
}

PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

PROCESS_THREAD(mqtt_publisher_process, ev, data) {
    static struct etimer timer;
    static uint16_t mid;

    PROCESS_BEGIN();

    // Initialize MQTT connection
    mqtt_register(&conn, PROCESS_CURRENT(), client_id, event_callback, 0);
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, NULL);

    etimer_set(&timer, 5 * CLOCK_SECOND);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Publish message
        mqtt_publish(&conn, &mid, TOPIC, (uint8_t *)message, strlen(message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_FALSE, 0, MQTT_TOPIC_ALIAS_EN_FALSE, NULL);

        etimer_reset(&timer);
    }

    PROCESS_END();
}
