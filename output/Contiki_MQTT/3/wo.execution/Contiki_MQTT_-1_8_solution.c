#include "sys/timer.h"
#include "net/ipv6/uip.h"
#include <string.h>
#include <stdio.h>
#include "mqtt.h"
#include "sys/ctimer.h"
#include "net/ipv6/uip-ds6.h"
#include "dev/gpio-hal.h"
#include "contiki-net.h"
#include "net/netstack.h"
#include "tcp-socket.h"

// Define variables and constants
static struct mqtt_connection conn;
static char client_id[32];
static char pub_topic[] = "sensor/status";
static char message[64];

static struct etimer periodic_timer; // Timer for periodic publishing

// MQTT event handler
void mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    switch (event) {
        case MQTT_EVENT_CONNECTED:
            printf("MQTT connected\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("MQTT disconnected\n");
            break;
        case MQTT_EVENT_PUBLISH:
            printf("MQTT publish received\n");
            break;
        default:
            break;
    }
}

// Main process thread
PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

PROCESS_THREAD(mqtt_publisher_process, ev, data) {
    PROCESS_BEGIN();

    // Initialize MQTT connection
    mqtt_register(&conn, &mqtt_publisher_process, client_id, mqtt_event, MAX_TCP_SEGMENT_SIZE_DEFAULT);
    mqtt_set_username_password(&conn, "username", "password");

    // Connect to MQTT broker
    mqtt_connect(&conn, "47.102.103.1", 1883, 60);

    etimer_set(&periodic_timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_YIELD();
        if (ev == PROCESS_EVENT_TIMER) {
            if (etimer_expired(&periodic_timer)) {
                snprintf(message, sizeof(message), "state: %s", "work");
                mqtt_publish(&conn, NULL, pub_topic, (uint8_t *)message, strlen(message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
                etimer_reset(&periodic_timer);
            }
        }
    }

    PROCESS_END();
}