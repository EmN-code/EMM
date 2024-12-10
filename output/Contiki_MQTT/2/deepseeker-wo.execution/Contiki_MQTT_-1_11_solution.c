
#include "contiki.h"
#include "sys/etimer.h"
#include "net/netstack.h"
#include "dev/gpio-hal.h"
#include "mqtt.h"
#include "mqtt-prop.h"
#include <string.h>
#include <stdio.h>

#define MQTT_BROKER_HOST "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "nrf52840_client"
#define MQTT_KEEP_ALIVE 60

static struct mqtt_connection conn;
static uint16_t mid;

PROCESS(mqtt_client_process, "MQTT Client Process");
AUTOSTART_PROCESSES(&mqtt_client_process);

static void event_callback(struct mqtt_connection *connection, mqtt_event_t event, void *data) {
    if (event == MQTT_EVENT_CONNECTED) {
        printf("MQTT connected\n");
    } else if (event == MQTT_EVENT_DISCONNECTED) {
        printf("MQTT disconnected\n");
    }
}

PROCESS_THREAD(mqtt_client_process, ev, data) {
    static struct etimer et;
    static char message[20];

    PROCESS_BEGIN();

    mqtt_register(&conn, PROCESS_CURRENT(), MQTT_CLIENT_ID, event_callback, 1024);
    mqtt_connect(&conn, MQTT_BROKER_HOST, MQTT_BROKER_PORT, MQTT_KEEP_ALIVE, 1, NULL);

    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        if (mqtt_connected(&conn)) {
            strcpy(message, "work");
            mqtt_publish(&conn, &mid, MQTT_TOPIC, (uint8_t *)message, strlen(message), MQTT_QOS_LEVEL_0, 0, 0, 0, NULL);
        }

        etimer_reset(&et);
    }

    PROCESS_END();
}
