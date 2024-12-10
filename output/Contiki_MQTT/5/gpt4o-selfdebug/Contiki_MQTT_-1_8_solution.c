#include "contiki.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/netstack.h"
#include "sys/etimer.h"
#include "mqtt.h"
#include <stdio.h>
#include <string.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)
#define TOPIC "nrf52840/status"

static struct mqtt_connection conn;
static struct etimer publish_timer;
static uint8_t state;

PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

static void mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    switch (event) {
        case MQTT_EVENT_CONNECTED:
            printf("MQTT connected\n");
            etimer_set(&publish_timer, PUBLISH_INTERVAL);
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("MQTT disconnected\n");
            process_exit(&mqtt_publisher_process);
            break;
        default:
            break;
    }
}

PROCESS_THREAD(mqtt_publisher_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_publisher_process, "nrf52840_client", mqtt_event, 512);
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 60);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));

        char message[20];
        if (state) {
            strcpy(message, "work");
        } else {
            strcpy(message, "not work");
        }

        mqtt_publish(&conn, NULL, TOPIC, (uint8_t *)message, strlen(message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
        printf("Published: %s\n", message);

        state = !state;
        etimer_reset(&publish_timer);
    }

    PROCESS_END();
}