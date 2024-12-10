
#include <stdio.h>
#include <string.h>
#include "net/mqtt.h"
#include "net/netstack.h"
#include "net/ipv6/uip.h"
#include "sys/etimer.h"
#include "sys/process.h"

#define MQTT_BROKER_HOST "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_KEEP_ALIVE 60
#define MQTT_QOS MQTT_QOS_LEVEL_0

static struct mqtt_connection mqtt_conn;
static struct etimer et;

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

static void mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    if (event == MQTT_EVENT_CONNECTED) {
        printf("MQTT connected\n");
    } else if (event == MQTT_EVENT_DISCONNECTED) {
        printf("MQTT disconnected\n");
    }
}

PROCESS_THREAD(mqtt_publish_process, ev, data) {
    static uint16_t mid;
    static char payload[] = MQTT_PAYLOAD;

    PROCESS_BEGIN();

    mqtt_register(&mqtt_conn, PROCESS_CURRENT(), "nrf52840_client", mqtt_event_callback, 1024);
    mqtt_connect(&mqtt_conn, MQTT_BROKER_HOST, MQTT_BROKER_PORT, MQTT_KEEP_ALIVE, 1, NULL);

    etimer_set(&et, 5 * CLOCK_SECOND);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        mqtt_publish(&mqtt_conn, &mid, MQTT_TOPIC, (uint8_t *)payload, strlen(payload), MQTT_QOS, 0, 0, 0, NULL);

        etimer_reset(&et);
    }

    PROCESS_END();
}
