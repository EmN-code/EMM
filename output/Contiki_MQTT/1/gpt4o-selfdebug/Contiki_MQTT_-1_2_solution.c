#include <stdio.h>
#include <string.h>
#include "net/netstack.h"
#include "mqtt.h"
#include "contiki.h"

// Define MQTT broker settings
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883

// MQTT connection parameters
static struct mqtt_connection conn;
static struct etimer periodic_timer;
static char client_id[] = "nrf52840_client";
static char pub_topic[] = "nrf52840/state";
static char state_message[] = "work";

PROCESS(nrf52840_mqtt_process, "nRF52840 MQTT Process");
AUTOSTART_PROCESSES(&nrf52840_mqtt_process);

PROCESS_THREAD(nrf52840_mqtt_process, ev, data) {
    PROCESS_BEGIN();

    // Connect to the MQTT broker
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 60 /*keep-alive interval*/);

    // Wait for connection
    while(uip_connected() == 0) {
        PROCESS_PAUSE();
    }

    // Publish state every 5 seconds
    etimer_set(&periodic_timer, CLOCK_SECOND * 5);
    while(1) {
        PROCESS_YIELD();
        if(etimer_expired(&periodic_timer)) {
            mqtt_publish(&conn, NULL, pub_topic, (uint8_t *)state_message, strlen(state_message), 0, 0);
            etimer_reset(&periodic_timer);
        }
    }

    PROCESS_END();
}