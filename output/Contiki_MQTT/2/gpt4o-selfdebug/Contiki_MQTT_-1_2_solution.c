// Import statements
#include "sensors.h"
#include <stdint.h>


// Required imports for the application
#include <stdio.h>
#include <stdbool.h>


#include <contiki.h>
#include <mqtt.h>


#include <string.h>
#include <stdlib.h>

// This program establishes a connection to an MQTT broker and publishes a "state"
// message (either "work" or "not work") every 5 seconds.

// Declaring the broker IP address and port
#define BROKER_IP_ADDR  "47.102.103.1"
#define BROKER_PORT     1883

// Client ID
#define CLIENT_ID       "nrf52840-mqtt-client"

// MQTT topic to publish
#define MQTT_TOPIC      "nrf52840/state"

// MQTT states
static char *mqtt_states[] = {"work", "not work"};

// Instantiate the necessary Contiki processes
PROCESS(mqtt_demo_process, "MQTT Demo Process");
AUTOSTART_PROCESSES(&mqtt_demo_process);

static struct mqtt_connection conn;
static struct etimer periodic_timer;
static int state_toggle = 0;

// Function to publish the MQTT message with the current state
t
void publish_state() {
    uint16_t mid;
    mqtt_status_t status;
    char *state = mqtt_states[state_toggle];

    // Publish MQTT message
    status = mqtt_publish(&conn, &mid, MQTT_TOPIC, (uint8_t *)state, strlen(state), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF, NO_TOPIC_ALIAS);

    if (status != MQTT_STATUS_OK) {
        printf("MQTT publish failed: %d\n", status);
    } else {
        printf("MQTT message published: %s\n", state);
    }

    state_toggle ^= 1; // Toggle state for the next message
}

PROCESS_THREAD(mqtt_demo_process, ev, data) {
    static bool connected = false;
    PROCESS_BEGIN();

    // Initialize the MQTT connection
    mqtt_register(&conn, &mqtt_demo_process, CLIENT_ID, NULL, MQTT_MAX_PACKET_SIZE);

    mqtt_connect(&conn, BROKER_IP_ADDR, BROKER_PORT, 1000,
                 MQTT_CLEAN_SESSION_ON);

    // Give time to connect
    etimer_set(&periodic_timer, CLOCK_SECOND * 3);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

    if (conn.state == MQTT_CONNECTED) {
        printf("Connected to MQTT broker\n");
        connected = true;
    } else {
        printf("Failed to connect to MQTT broker\n");
        PROCESS_EXIT();
    }

    // Set the periodic timer
    etimer_set(&periodic_timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT();

        if (etimer_expired(&periodic_timer)) {
            if (connected) {
                publish_state();
            }

            // Reset the timer
            etimer_reset(&periodic_timer);
        }
    }

    PROCESS_END();
}