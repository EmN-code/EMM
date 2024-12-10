
#include <stdio.h>
#include <string.h>
#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ip/tcpip.h"
#include "sys/etimer.h"
#include "dev/leds.h"
#include "mqtt.h"
#include "mqtt-transport.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "nrf52840_client"

static struct mqtt_connection conn;
static struct etimer et;

static void mqtt_event_callback(struct mqtt_connection *c, mqtt_event_t event, void *data) {
    if (event == MQTT_EVENT_CONNECT) {
        printf("MQTT connected\n");
    } else if (event == MQTT_EVENT_DISCONNECT) {
        printf("MQTT disconnected\n");
    }
}

static void publish_state(void) {
    static int state = 0;
    char message[20];
    sprintf(message, "work or not work: %s", state ? "work" : "not work");
    state = !state;

    mqtt_publish(&conn, NULL, MQTT_TOPIC, (uint8_t *)message, strlen(message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_FALSE, 0, MQTT_TOPIC_ALIAS_EN_FALSE, NULL);
}

PROCESS(mqtt_client_process, "MQTT Client Process");
AUTOSTART_PROCESSES(&mqtt_client_process);

PROCESS_THREAD(mqtt_client_process, ev, data) {
    PROCESS_BEGIN();

    tcpip_init(NULL, NULL);

    mqtt_register(&conn, PROCESS_CURRENT(), MQTT_CLIENT_ID, mqtt_event_callback, 1024);
    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, NULL);

    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        etimer_reset(&et);
        publish_state();
    }

    PROCESS_END();
}
