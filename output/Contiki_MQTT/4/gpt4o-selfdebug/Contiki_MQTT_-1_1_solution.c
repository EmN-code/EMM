#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "mqtt.h"
#include "contiki.h"

PROCESS(mqtt_client_process, "MQTT Client");
AUTOSTART_PROCESSES(&mqtt_client_process);

static struct etimer periodic_timer;
static char broker_ip[] = "47.102.103.1";
static uint16_t port = 1883;
static char client_id[] = "nrf52840_client";
static struct mqtt_connection conn;

void publish_state(const char *state) {
    mqtt_publish(&conn, NULL, "nrf52840/status", (uint8_t *)state, strlen(state), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
}

static void mqtt_event_handler(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    switch (event) {
        case MQTT_EVENT_CONNECTED:
            printf("Connected to MQTT broker\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("Disconnected from MQTT broker\n");
            break;
        case MQTT_EVENT_PUBLISH:
            printf("Message published\n");
            break;
        case MQTT_EVENT_SUBACK:
            printf("Subscribe acknowledged\n");
            break;
        default:
            printf("Unknown MQTT event: %d\n", event);
            break;
    }
}

PROCESS_THREAD(mqtt_client_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_client_process, client_id, mqtt_event_handler, 128);

    mqtt_connect(&conn, broker_ip, port, 1000 * 60 * 5);

    etimer_set(&periodic_timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

        if (mqtt_connected(&conn)) {
            publish_state("work");
        } else {
            publish_state("not work");
        }

        etimer_reset(&periodic_timer);
    }

    PROCESS_END();
}