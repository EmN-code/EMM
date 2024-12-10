#include "net/ipv6/uip-nameserver.h"

#include "contiki.h"
#include "mqtt.h"
#include <stdio.h>
#include <string.h>

/* MQTT configuration values */
#define BROKER_IP_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID    "nrf52_client"
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)
#define TOPIC "nrf52/state"
#define MESSAGE "work or not work"

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

static struct mqtt_connection conn;
static struct etimer publish_timer;

/* MQTT event handler */
static void mqtt_event_callback(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    if (event == MQTT_EVENT_CONNECTED) {
        printf("Connected to the broker at %s:%d\n", BROKER_IP_ADDR, BROKER_PORT);
    } else if (event == MQTT_EVENT_DISCONNECTED) {
        printf("Disconnected from the broker\n");
    }
}

PROCESS_THREAD(mqtt_example_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_example_process, CLIENT_ID, mqtt_event_callback, MAX_TCP_SEGMENT_SIZE);
    mqtt_connect(&conn, BROKER_IP_ADDR, BROKER_PORT, PUBLISH_INTERVAL, 0);

    etimer_set(&publish_timer, PUBLISH_INTERVAL);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));
        mqtt_publish(&conn, NULL, TOPIC, (uint8_t *)MESSAGE, strlen(MESSAGE), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
        printf("Published message: %s\n", MESSAGE);
        etimer_reset(&publish_timer);
    }

    PROCESS_END();
}
