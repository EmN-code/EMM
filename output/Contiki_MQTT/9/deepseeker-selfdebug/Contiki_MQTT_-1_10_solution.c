
#include <stdio.h>
#include <string.h>
#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ip/uip-ds6.h"
#include "sys/etimer.h"
#include "mqtt.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "nrf52840_client"

static struct mqtt_connection conn;
static struct etimer et;

static void mqtt_event_callback(struct mqtt_connection *connection, mqtt_event_t event, void *data) {
    if (event == MQTT_EVENT_CONNECTED) {
        printf("MQTT connected\n");
    } else if (event == MQTT_EVENT_DISCONNECTED) {
        printf("MQTT disconnected\n");
    }
}

static void publish_state(struct mqtt_connection *connection, const char *state) {
    mqtt_publish(connection, MQTT_TOPIC, state, strlen(state), MQTT_QOS_0, 0);
}

PROCESS(mqtt_client_process, "MQTT Client Process");
AUTOSTART_PROCESSES(&mqtt_client_process);

PROCESS_THREAD(mqtt_client_process, ev, data) {
    static char state[] = "work";
    static int toggle = 0;

    PROCESS_BEGIN();

    mqtt_register(&conn, PROCESS_CURRENT(), MQTT_CLIENT_ID, mqtt_event_callback, 0);
    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 0, NULL);

    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        if (toggle) {
            strcpy(state, "work");
        } else {
            strcpy(state, "not work");
        }
        toggle = !toggle;

        publish_state(&conn, state);
        etimer_reset(&et);
    }

    PROCESS_END();
}
