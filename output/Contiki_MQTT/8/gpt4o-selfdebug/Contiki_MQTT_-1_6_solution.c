#include "contiki.h"
#include <stdio.h>
#include "sys/etimer.h"
#include "mqtt.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (CLOCK_SECOND * 5)

static struct mqtt_connection conn;
static struct etimer publish_timer;
char *topic = "device/status";
char *message = "work";
uint16_t mid;

PROCESS(mqtt_publisher, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher);

PROCESS_THREAD(mqtt_publisher, ev, data) {
    PROCESS_BEGIN();
    
    printf("Starting MQTT Publisher\n");
    mqtt_register(&conn, &mqtt_publisher, mqtt_con_cb, NULL);
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, PUBLISH_INTERVAL);

    etimer_set(&publish_timer, PUBLISH_INTERVAL);

    while(1) {
        PROCESS_YIELD();

        if(etimer_expired(&publish_timer)) {
            mqtt_publish(&conn, &mid, topic, (uint8_t *)message, strlen(message), MQTT_QOS_LEVEL_0);
            printf("Published message: %s\n", message);
            etimer_reset(&publish_timer);
        }
    }

    PROCESS_END();
}

void mqtt_con_cb(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    switch(event) {
        case MQTT_EVENT_CONNECTED:
            printf("MQTT connected\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("MQTT disconnected\n");
            process_exit(&mqtt_publisher);
            break;
        default:
            printf("MQTT event: %d\n", event);
            break;
    }
}