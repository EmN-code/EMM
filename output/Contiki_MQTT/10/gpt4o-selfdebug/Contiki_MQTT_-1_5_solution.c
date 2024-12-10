#include <stdio.h>
#include "net/ipv6/simple-udp.h"
#include "mqtt.h"

#define BROKER_IP_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "nrf52840-client"

#define TOPIC "state"
#define PUBLISH_INTERVAL 5
#define MQTT_MAX_SEGMENT_SIZE 128

PROCESS(mqtt_example, "MQTT Example");
AUTOSTART_PROCESSES(&mqtt_example);

static struct mqtt_connection conn;
static struct etimer publish_timer;

PROCESS_THREAD(mqtt_example, ev, data)
{
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_example, CLIENT_ID, mqtt_event_callback, MQTT_MAX_SEGMENT_SIZE);
    mqtt_connect(&conn, BROKER_IP_ADDR, BROKER_PORT, 60, MQTT_CLEAN_SESSION, NULL);

    while (1) {
        etimer_set(&publish_timer, CLOCK_SECOND * PUBLISH_INTERVAL);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));

        mqtt_publish(&conn, NULL, TOPIC, (uint8_t *)"work", sizeof("work"), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF, 0, MQTT_TOPIC_ALIAS_NOT_SET, NULL);
    }

    PROCESS_END();
}

void mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    switch (event) {
        case MQTT_EVENT_CONNECTED:
            printf("MQTT connected\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("MQTT disconnected\n");
            break;
        default:
            break;
    }
}