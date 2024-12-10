
#include "contiki.h"
#include "sys/etimer.h"
#include "string.h"
#include "net/netstack.h"
#include "sys/process.h"
#include "sys/clock.h"
#include "mqtt-prop.h"
#include "stdbool.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS_LEVEL 0

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

static struct mqtt_connection conn;
static struct etimer et;

void mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event) {
    if (event == MQTT_EVENT_CONNECTED) {
        printf("MQTT connected\n");
    } else if (event == MQTT_EVENT_DISCONNECTED) {
        printf("MQTT disconnected\n");
    }
}

PROCESS_THREAD(mqtt_example_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, PROCESS_CURRENT(), "nrf52840_client", mqtt_event_callback, 128);
    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60, 1, NULL);

    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        if (mqtt_connected(&conn)) {
            mqtt_publish(&conn, NULL, MQTT_TOPIC, (uint8_t *)MQTT_PAYLOAD, strlen(MQTT_PAYLOAD), MQTT_QOS_LEVEL, 0, 0, 0, NULL);
        }

        etimer_reset(&et);
    }

    PROCESS_END();
}
