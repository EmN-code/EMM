#include "sys/process.h"
#include "net/netstack.h"
#include "net/ipv6/uip.h"
#include "mqtt.h"
#include "mqtt-sensors.h"
#include "random.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)
#define CLIENT_ID "nrf52840_client"
#define STATE_TOPIC "device/state"

static struct mqtt_connection conn;
static struct etimer periodic_timer;
static char *state_message = "work";

// MQTT event handler
static void mqtt_event_handler(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    switch(event) {
        case MQTT_EVENT_CONNECTED: {
            printf("Connected to MQTT broker\n");
            break;
        }
        case MQTT_EVENT_DISCONNECTED: {
            printf("Disconnected from MQTT broker\n");
            break;
        }
        case MQTT_EVENT_PUBLISH: {
            printf("Message published\n");
            break;
        }
        case MQTT_EVENT_SUBACK: {
            printf("Subscription acknowledged\n");
            break;
        }
        default: {
            printf("Unhandled MQTT event: %d\n", event);
            break;
        }
    }
}

PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

PROCESS_THREAD(mqtt_publisher_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_publisher_process, CLIENT_ID, mqtt_event_handler, MAX_TCP_SEGMENT_SIZE);
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 60 * CLOCK_SECOND);

    etimer_set(&periodic_timer, PUBLISH_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
        printf("Publishing state: %s\n", state_message);
        mqtt_publish(&conn, NULL, STATE_TOPIC, (uint8_t *)state_message, strlen(state_message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
        etimer_reset(&periodic_timer);
    }

    PROCESS_END();
}