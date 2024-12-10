
#include "contiki.h"
#include "net/mqtt.h"  // Ensure this header file is included
#include <stdio.h>
#include <string.h>

// Define the MQTT broker details
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"

// MQTT connection structure
static struct mqtt_connection conn;

// Function to publish state
void publish_state(void) {
    static const char *states[] = {"work", "not work"};
    static uint8_t state_index = 0;

    char payload[10];
    snprintf(payload, sizeof(payload), "%s", states[state_index]);
    state_index = (state_index + 1) % 2;

    mqtt_publish(&conn, NULL, TOPIC, (uint8_t *)payload, strlen(payload), MQTT_QOS_LEVEL_0, MQTT_RETAIN_FALSE, 0, MQTT_TOPIC_ALIAS_EN_FALSE, NULL);
}

// Process to handle MQTT events
PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

PROCESS_THREAD(mqtt_example_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    // Initialize MQTT connection
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 60, MQTT_CLEAN_SESSION_TRUE, NULL);

    // Set up a periodic timer to publish state every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Publish the state
        publish_state();

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void) {
    // Start the Contiki system
    process_start(&mqtt_example_process, NULL);

    // Enter the Contiki main loop
    while (1) {
        process_run();
    }

    return 0;
}
