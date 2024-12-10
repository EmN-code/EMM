
#include "contiki.h"
#include "net/mqtt.h"  // Ensure this header file is included
#include <stdio.h>

// Define the MQTT broker details
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"

// Define the MQTT connection structure
struct mqtt_connection conn;

// Function to publish the state
void publish_state(struct mqtt_connection *conn) {
    static int state = 0;
    char message[20];
    sprintf(message, "work or not work: %s", state ? "work" : "not work");
    state = !state;

    mqtt_publish(conn, NULL, TOPIC, (uint8_t *)message, strlen(message), MQTT_QOS_LEVEL_0, 0, 0, NULL);
}

// Process to handle MQTT connection and periodic publishing
PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

PROCESS_THREAD(mqtt_example_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    // Initialize the MQTT connection
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 60, 1, NULL);

    // Set up a periodic timer to publish every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Publish the state
        publish_state(&conn);

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main() {
    // Start the Contiki system
    process_start(&mqtt_example_process, NULL);

    // Enter the Contiki main loop
    while (1) {
        process_run();
    }

    return 0;
}
