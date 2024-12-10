#include "sys/etimer.h"
#include "net/ipv6/uip.h"
#include "mqtt.h"
#include "contiki.h"

#include <string.h>
#include <stdio.h>

#define STATE_INTERVAL (5 * CLOCK_SECOND)
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "nrf52840-client"
#define PUBLISH_TOPIC "nrf52840/status"

PROCESS(mqtt_nrf52840_process, "NRF52840 MQTT Publisher");
AUTOSTART_PROCESSES(&mqtt_nrf52840_process);

static struct etimer state_timer;
static struct mqtt_connection conn;

static void mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    if(event == MQTT_EVENT_CONNECTED) {
        printf("Connected to MQTT broker\n");
    }
    else if(event == MQTT_EVENT_DISCONNECTED) {
        printf("Disconnected from MQTT broker\n");
    }
}

PROCESS_THREAD(mqtt_nrf52840_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_nrf52840_process, CLIENT_ID, mqtt_event, 256);
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, STATE_INTERVAL * 2);

    etimer_set(&state_timer, STATE_INTERVAL);

    while(1) {
        PROCESS_YIELD();

        if(etimer_expired(&state_timer)) {
            static char msg[20];
            snprintf(msg, sizeof(msg), "State: work");
            mqtt_publish(&conn, NULL, PUBLISH_TOPIC, (uint8_t *)msg, strlen(msg), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);

            etimer_reset(&state_timer);
        }
    }

    PROCESS_END();
}