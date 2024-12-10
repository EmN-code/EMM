
#include "contiki.h"
#include "net/mqtt.h"
#include "sys/etimer.h"
#include <stdio.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"

static struct mqtt_connection conn;
static struct etimer et;

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

static void event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    if (event == MQTT_EVENT_CONNECTED) {
        printf("Connected to MQTT broker\n");
    } else if (event == MQTT_EVENT_DISCONNECTED) {
        printf("Disconnected from MQTT broker\n");
    }
}

PROCESS_THREAD(mqtt_example_process, ev, data) {
    static uint16_t mid;
    static uint8_t payload[] = PAYLOAD;
    static uint32_t payload_size = sizeof(payload) - 1;

    PROCESS_BEGIN();

    mqtt_register(&conn, PROCESS_CURRENT(), "client_id", event_callback, 128);
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 60, 1, NULL);

    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        mqtt_publish(&conn, &mid, TOPIC, payload, payload_size, MQTT_QOS_LEVEL_0, 0, 0, MQTT_TOPIC_ALIAS_EN_NO, NULL);

        etimer_reset(&et);
    }

    PROCESS_END();
}
