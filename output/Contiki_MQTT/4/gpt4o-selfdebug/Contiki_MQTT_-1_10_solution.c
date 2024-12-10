#include <net/ipv6/uip-ds6-nbr.h>
#include <mqtt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/etimer.h>
#include <sys/ctimer.h>
#include <contiki.h>

/* Configure MQTT settings */
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (CLOCK_SECOND * 5)
#define MQTT_CLIENT_ID "nrf52840_mqtt_client"
#define MQTT_TOPIC "device/state"
#define MQTT_PAYLOAD "work or not work"

PROCESS(nrf52840_mqtt_process, "NRF52840 MQTT Process");
AUTOSTART_PROCESSES(&nrf52840_mqtt_process);

static struct etimer publish_timer;
static struct mqtt_connection conn;

static void mqtt_event_callback(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    if (event == MQTT_EVENT_CONNECTED) {
        printf("Connected to MQTT broker\n");
    }
    else if (event == MQTT_EVENT_DISCONNECTED) {
        printf("Disconnected from MQTT broker\n");
    }
}

PROCESS_THREAD(nrf52840_mqtt_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, &nrf52840_mqtt_process, MQTT_CLIENT_ID, mqtt_event_callback, 100);
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, PUBLISH_INTERVAL);

    etimer_set(&publish_timer, PUBLISH_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));

        mqtt_publish(&conn, NULL, MQTT_TOPIC, (uint8_t *)MQTT_PAYLOAD, strlen(MQTT_PAYLOAD), MQTT_QOS_LEVEL_0);
        printf("Published: %s\n", MQTT_PAYLOAD);

        etimer_reset(&publish_timer);
    }

    PROCESS_END();
}

int main(void) {
    process_start(&nrf52840_mqtt_process, NULL);
    while(1) {
        process_run();
    }
}