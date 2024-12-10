#include "contiki.h"
#include "mqtt.h"
#include <stdio.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "nrf52840"
#define PUBLISH_TOPIC "device/state"
#define MQTT_KEEP_ALIVE 60

static struct mqtt_connection conn;
static struct etimer publish_timer;
static char *state_msg[2] = {"not work", "work"};
static int state = 0;

PROCESS(mqtt_publisher_process, "MQTT Publisher");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

static void mqtt_event_callback(struct mqtt_connection *m, mqtt_event_t event, void *data)
{
    switch (event) {
        case MQTT_EVENT_CONNECTED:
            printf("Connected to MQTT broker\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("Disconnected from MQTT broker\n");
            break;
        case MQTT_EVENT_PUBLISH:
            printf("Publish sent\n");
            break;
        default:
            printf("MQTT event handled: %d\n", event);
            break;
    }
}

PROCESS_THREAD(mqtt_publisher_process, ev, data)
{
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_publisher_process, CLIENT_ID, mqtt_event_callback, MQTT_KEEP_ALIVE);
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, MQTT_KEEP_ALIVE);

    etimer_set(&publish_timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));

        mqtt_publish(&conn, NULL, PUBLISH_TOPIC, (uint8_t *)state_msg[state], 
                    strlen(state_msg[state]), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);

        state = 1 - state; // toggle state between 0 and 1
        etimer_reset(&publish_timer);
    }

    PROCESS_END();
}
