
#include "contiki.h"
#include "net/netstack.h"
#include "sys/etimer.h"
#include "dev/leds.h"
#include "lib/random.h"
#include "mqtt.h"
#include "mqtt-transport.h"
#include "mqtt-packet.h"
#include "mqtt-strings.h"
#include "mqtt-client.h"

#include <stdio.h>
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_CLIENT_ID "nrf52840_client"

static struct mqtt_connection conn;
static struct etimer et;

void mqtt_event_handler(struct mqtt_connection *c, mqtt_event_t event, void *data) {
    if (event == MQTT_EVENT_CONNECTED) {
        printf("MQTT connected\n");
    } else if (event == MQTT_EVENT_DISCONNECTED) {
        printf("MQTT disconnected\n");
    }
}

void publish_state(struct mqtt_connection *c) {
    static int state = 0;
    const char *states[] = {"work", "not work"};
    char payload[20];

    snprintf(payload, sizeof(payload), "%s", states[state]);
    state = !state; // Toggle state

    mqtt_publish(c, MQTT_TOPIC, payload, strlen(payload), 0, 0);
    printf("Published: %s\n", payload);
}

PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

PROCESS_THREAD(mqtt_publisher_process, ev, data) {
    PROCESS_BEGIN();

    // Initialize MQTT connection
    mqtt_init(&conn, MQTT_CLIENT_ID, NULL, NULL, mqtt_event_handler);
    mqtt_transport_init(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    // Connect to MQTT broker
    mqtt_connect(&conn);

    // Set timer for periodic publishing
    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        if (conn.state == MQTT_STATE_CONNECTED) {
            publish_state(&conn);
        }

        etimer_reset(&et);
    }

    PROCESS_END();
}
