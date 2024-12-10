# Define the necessary imports
#include "sys/etimer.h"
#include "net/ipv6/uip.h"
#include "net/ip/uip-udp-packet.h"
#include "net/mqtt.h"
#include "net/ip/uip-debug.h"
#include "stdio.h"
#include "stdlib.h"


// Declare a process for the MQTT client connection
PROCESS(mqtt_client_process, "MQTT Client");
AUTOSTART_PROCESSES(&mqtt_client_process);

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883

#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

// Buffer for the MQTT message
static char pub_message[50];

// Event timer for publishing at regular intervals
static struct etimer publish_timer;

void mqtt_event_callback(struct mqtt_connection* conn, mqtt_event_t event, void* data) {
    // Handle different MQTT events here
    switch (event) {
    case MQTT_EVENT_CONNECTED:
        printf("Connected to MQTT broker\n");
        break;

    case MQTT_EVENT_DISCONNECTED:
        printf("Disconnected from MQTT broker\n");
        break;

    case MQTT_EVENT_PUBLISHED:
        printf("Published to MQTT broker\n");
        break;

    default:
        printf("MQTT event: %d\n", event);
        break;
    }
}

PROCESS_THREAD(mqtt_client_process, ev, data) {

    // Declare process variables
    static struct mqtt_connection conn;

    PROCESS_BEGIN();

    // Initialize the MQTT connection
    mqtt_register(&conn, &mqtt_client_process, BROKER_IP, mqtt_event_callback);
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 1000);  // Timeout after 1 second

    // Set the publish timer
    etimer_set(&publish_timer, PUBLISH_INTERVAL);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));

        // Create an MQTT message
        snprintf(pub_message, sizeof(pub_message), "status:work");

        // Publish the message
        mqtt_publish(&conn, NULL, "status", (uint8_t*)pub_message, strlen(pub_message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);

        // Reset the timer for the next publication
        etimer_reset(&publish_timer);
    }

    PROCESS_END();
}