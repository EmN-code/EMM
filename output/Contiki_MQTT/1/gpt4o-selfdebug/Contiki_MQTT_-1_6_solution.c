#include "net/mac/framer/framer.h"
#include "mqtt.h"
#include "contiki.h"
#include <stdio.h>

PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

static struct etimer timer;
static struct mqtt_connection conn;
static char *broker_ip = "47.102.103.1";
static uint16_t broker_port = 1883;
static char *client_id = "contiki_nrf52840_client";
static char *topic = "nrf52840/state";

void mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data) {
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

PROCESS_THREAD(mqtt_publisher_process, ev, data) {
    PROCESS_BEGIN();

    static char message[20];
    mqtt_register(&conn, &mqtt_publisher_process, client_id, mqtt_event, 128);
    etimer_set(&timer, CLOCK_SECOND * 5);

    mqtt_connect(&conn, broker_ip, broker_port, 100);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        snprintf(message, sizeof(message), "work");
        mqtt_publish(&conn, NULL, topic, (uint8_t *)message, strlen(message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
        etimer_reset(&timer);
    }

    PROCESS_END();
}