#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mqtt.h"
#include "etimer.h"
#include "clock.h"

static struct mqtt_connection conn;
static struct etimer et;

#define MQTT_BROKER_IPADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)  // 5 seconds

static char broker_address[64];
static const char *pub_topic = "device/state";

void publish_state(const char *state) {
    printf("Publishing state: %s\n", state);
    clock_time_t ct = clock_time(); // Ensure consistent timing
    static char payload[64];
    snprintf(payload, sizeof(payload), "State: %s at %lu", state, ct);
    mqtt_publish(&conn, NULL, pub_topic, (uint8_t *)payload, strlen(payload), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
}

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

PROCESS_THREAD(mqtt_example_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_example_process, "nrf52840", mqtt_event, 255);

    snprintf(broker_address, sizeof(broker_address), "mqtt://%s:%d", MQTT_BROKER_IPADDR, MQTT_BROKER_PORT);
    mqtt_connect(&conn, broker_address, 1883, 1000);

    etimer_set(&et, PUBLISH_INTERVAL);
    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        publish_state("work");
        etimer_reset(&et);
    }

    PROCESS_END();
}

void mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    switch (event) {
        case MQTT_EVENT_CONNECTED:
            printf("Connected to MQTT broker at %s:%d\n", MQTT_BROKER_IPADDR, MQTT_BROKER_PORT);
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("Disconnected from MQTT broker\n");
            process_exit(&mqtt_example_process);
            break;
        default:
            break;
    }
}