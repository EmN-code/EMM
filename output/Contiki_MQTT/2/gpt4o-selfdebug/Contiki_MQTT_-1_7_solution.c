#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "mqtt.h"
#include "net/ipv6/uiplib.h"
#include "sys/etimer.h"
#include "contiki.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)
#define TOPIC "state"
#define CLIENT_ID "nrf52840_client"

static struct etimer periodic_timer;
static struct mqtt_connection conn;

PROCESS(mqtt_demo_process, "MQTT Demo");
AUTOSTART_PROCESSES(&mqtt_demo_process);

static void mqtt_event_handler(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    switch(event) {
    case MQTT_EVENT_CONNECTED:
        printf("MQTT connected\n");
        break;
    case MQTT_EVENT_DISCONNECTED:
        printf("MQTT disconnected\n");
        break;
    case MQTT_EVENT_PUBLISH:
        printf("MQTT data published\n");
        break;
    default:
        printf("MQTT event 0x%02x\n", event);
        break;
    }
}

PROCESS_THREAD(mqtt_demo_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_demo_process, CLIENT_ID, mqtt_event_handler, 256);
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 120);

    etimer_set(&periodic_timer, PUBLISH_INTERVAL);

    while(1) {
        PROCESS_YIELD();

        if(etimer_expired(&periodic_timer)) {
            char state[] = "work or not work";
            mqtt_publish(&conn, NULL, TOPIC, (uint8_t *)state, strlen(state), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
            etimer_reset(&periodic_timer);
        }
    }

    PROCESS_END();
}