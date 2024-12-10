#include <stdio.h>
#include <string.h>
#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "mqtt.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "nrf52840_client"
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work or not work"
#define PUBLISH_INTERVAL 5

static struct mqtt_connection conn;

void mqtt_event_callback(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    // Handling MQTT events like connect, disconnect, publish, etc.
}

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

PROCESS_THREAD(mqtt_publish_process, ev, data) {
    static struct etimer publish_timer;
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_publish_process, MQTT_CLIENT_ID, mqtt_event_callback, MAX_TCP_SEGMENT_SIZE);
    mqtt_set_username_password(&conn, "", "");

    // MQTT connection attempt
    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, KEEP_ALIVE_INTERVAL, MQTT_CLEAN_SESSION);

    etimer_set(&publish_timer, CLOCK_SECOND * PUBLISH_INTERVAL);

    while(1) {
        PROCESS_YIELD();

        if (ev == PROCESS_EVENT_TIMER) {
            if (mqtt_connected(&conn)) {
                static uint16_t msg_id;
                mqtt_publish(&conn, &msg_id, MQTT_TOPIC, MQTT_MESSAGE, MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
            }
            etimer_reset(&publish_timer);
        }
    }

    PROCESS_END();
}