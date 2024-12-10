#include "net/netstack.h"
#include "sys/etimer.h"
#include "mqtt.h"
#include "contiki.h"
#include <stdio.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (CLOCK_SECOND * 5)

PROCESS(mqtt_publisher_process, "MQTT Publisher");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

static struct etimer publish_timer;
static struct mqtt_connection conn;

static void on_mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    switch(event) {
        case MQTT_EVENT_CONNECTED:
            printf("Connected to the MQTT broker\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("Disconnected from the MQTT broker\n");
            break;
        case MQTT_EVENT_PUBLISH_ACK:
            printf("Publish acknowledged\n");
            break;
        default:
            printf("Other MQTT event: %d\n", event);
            break;
    }
}

PROCESS_THREAD(mqtt_publisher_process, ev, data) {
    PROCESS_BEGIN();
    
    printf("MQTT Publisher Example\n");

    mqtt_register(&conn, &mqtt_publisher_process, "publisher", on_mqtt_event, 512);
    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, (PUBLISH_INTERVAL * 3) / CLOCK_SECOND);

    etimer_set(&publish_timer, PUBLISH_INTERVAL);

    while(1) {
        PROCESS_YIELD();

        if(ev == PROCESS_EVENT_TIMER && data == &publish_timer) {
            static char message[20];
            snprintf(message, sizeof(message), "work");

            mqtt_publish(&conn, NULL, "state", (uint8_t *)message, strlen(message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);

            printf("Published: %s\n", message);

            etimer_reset(&publish_timer);
        }
    }

    PROCESS_END();
}