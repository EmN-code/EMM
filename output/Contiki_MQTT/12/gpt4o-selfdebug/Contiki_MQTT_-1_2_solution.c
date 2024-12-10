/*
  This program is intended to be run on an NRF52840 using the Contiki OS. It connects to an MQTT broker at the specified address and port
  (47.102.103.1:1883) and publishes a state message every five seconds.
*/

// Import necessary Contiki and MQTT libraries
#include "mqtt.h"
#include "contiki.h"
#include "contiki-net.h"

// Define constants for MQTT
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUBLISH_INTERVAL (CLOCK_SECOND * 5)
#define MQTT_CLIENT_ID "nrf52840_client"
#define MQTT_TOPIC "state/topic"

// Define process
PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

// MQTT buffers
static struct mqtt_connection conn;

// Define function to publish state
static void publish_state(void) {
    uint16_t mid;
    static char state_message[16];

    // Placeholder for state fetching logic
    snprintf(state_message, sizeof(state_message), "work"); // Example state

    // Publishing the state
    mqtt_publish(&conn, &mid, MQTT_TOPIC, (uint8_t *)state_message, strlen(state_message),
                 MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
}

// Implementation of the process
PROCESS_THREAD(mqtt_publish_process, ev, data) {
    PROCESS_BEGIN();

    // Initialize MQTT connection
    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, MQTT_PUBLISH_INTERVAL / CLOCK_SECOND,
                 MQTT_CLEAN_SESSION_ON); // Ensure clean session is enabled

    static struct etimer publish_timer;
    while (1) {
        // Set the timer for the publish interval
        etimer_set(&publish_timer, MQTT_PUBLISH_INTERVAL);

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));
        publish_state(); // Publish state when the timer expires
    }

    PROCESS_END();
}