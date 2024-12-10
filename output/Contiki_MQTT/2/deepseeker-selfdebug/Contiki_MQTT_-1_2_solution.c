
#include <stdio.h>
#include <string.h>
#include "net/mqtt.h"
#include "net/ipv6/uip.h"
#include "net/netstack.h"
#include "sys/etimer.h"
#include "sys/process.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD_WORK "work"
#define PAYLOAD_NOT_WORK "not work"
#define INTERVAL (5 * CLOCK_SECOND)

static struct mqtt_connection conn;
static struct etimer et;
static uint16_t mid;

static void mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    if (event == MQTT_EVENT_CONNECT) {
        printf("Connected to MQTT broker\n");
    } else if (event == MQTT_EVENT_DISCONNECT) {
        printf("Disconnected from MQTT broker\n");
    }
}

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

PROCESS_THREAD(mqtt_publish_process, ev, data) {
    static bool state = true;

    PROCESS_BEGIN();

    mqtt_register(&conn, PROCESS_CURRENT(), "nrf52840_client", mqtt_event_callback, 0);
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, NULL);

    etimer_set(&et, INTERVAL);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        const char *payload = state ? PAYLOAD_WORK : PAYLOAD_NOT_WORK;
        mqtt_publish(&conn, &mid, TOPIC, (uint8_t *)payload, strlen(payload), MQTT_QOS_LEVEL_0, MQTT_RETAIN_FALSE, 0, MQTT_TOPIC_ALIAS_EN_FALSE, NULL);

        state = !state;
        etimer_reset(&et);
    }

    PROCESS_END();
}
