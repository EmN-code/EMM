
#include "contiki.h"
#include "net/netstack.h"
#include "net/ipv6/uip.h"
#include "lib/random.h"
#include "dev/leds.h"
#include "sys/etimer.h"
#include "sys/clock.h"
#include "mqtt.h"
#include "mqtt-transport.h"
#include "mqtt-packet.h"
#include "mqtt-strings.h"
#include "mqtt-client.h"

#include <stdio.h>
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
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

void publish_state(struct mqtt_connection *c, const char *state) {
    mqtt_publish(c, MQTT_TOPIC, state, strlen(state), 0, 0);
}

PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

PROCESS_THREAD(mqtt_publisher_process, ev, data) {
    static struct mqtt_connect_info connect_info = {
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = 60,
        .clean_session = 1,
        .will_topic = NULL,
        .will_message = NULL,
        .will_qos = 0,
        .will_retain = 0,
        .username = NULL,
        .password = NULL
    };

    PROCESS_BEGIN();

    printf("Connecting to MQTT broker at %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);

    mqtt_init(&conn, NULL, mqtt_event_handler, NULL);
    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, &connect_info);

    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        if (conn.state == MQTT_STATE_CONNECTED) {
            const char *state = (random_rand() % 2 == 0) ? "work" : "not work";
            printf("Publishing state: %s\n", state);
            publish_state(&conn, state);
        }

        etimer_reset(&et);
    }

    PROCESS_END();
}
