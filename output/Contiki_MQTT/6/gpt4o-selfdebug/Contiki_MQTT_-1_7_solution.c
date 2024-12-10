#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"

#include "mqtt.h"
#include "contiki.h"
#include <stdio.h>
#include <string.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_PUBLISH_INTERVAL (CLOCK_SECOND * 5)
#define MQTT_PUBLISH_TOPIC "device/state"

static struct mqtt_connection conn;
static struct etimer publish_timer;

void mqtt_event_callback(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    switch(event) {
        case MQTT_EVENT_CONNECTED:
            printf("MQTT connected\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("MQTT disconnected\n");
            break;
        case MQTT_EVENT_PUBLISHED:
            printf("MQTT published\n");
            break;
        default:
            printf("MQTT event: %d\n", event);
            break;
    }
}

void init_mqtt() {
    mqtt_register(&conn, &mqtt_event_callback);
    mqtt_set_last_will(&conn, MQTT_PUBLISH_TOPIC, "not work", NULL, MQTT_QOS_LEVEL_0);
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, MQTT_PUBLISH_INTERVAL * 3, MQTT_CLEAN_SESSION_ON);
}

PROCESS(mqtt_publish_process, "MQTT Publish process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

PROCESS_THREAD(mqtt_publish_process, ev, data) {
    PROCESS_BEGIN();

    etimer_set(&publish_timer, MQTT_PUBLISH_INTERVAL);
    init_mqtt();

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));

        mqtt_publish(&conn, NULL, MQTT_PUBLISH_TOPIC, (uint8_t *)"work", strlen("work"), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);

        etimer_reset(&publish_timer);
    }

    PROCESS_END();
}