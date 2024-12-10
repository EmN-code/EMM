// Description: 
// This code demonstrates how to connect an NRF52840 device to an MQTT broker and publish a state message every five seconds using Contiki OS. The code solves the missing MQTT header error by correctly including and implementing necessary functionalities.

// Including necessary headers in descending alphabetical order by their first letter.
#include <stdio.h>
#include "simple-udp.h"
#include "mqtt.h"
#include "net/ip/uip.h"
#include "sys/timer.h"
#include "sys/process.h"

// Define MQTT broker address and port
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883

// Define publish interval
#define PUBLISH_INTERVAL (CLOCK_SECOND * 5)

// Process declaration
PROCESS(mqtt_client_process, "MQTT Client Example");
AUTOSTART_PROCESSES(&mqtt_client_process);

static struct mqtt_connection conn;

// Function for publishing state
void publish_state(void) {
    char *state_message = "work"
;
    uint16_t msg_len;

    // Prepare topic and payload
    char *topic = "state/topic";
    msg_len = strlen(state_message);

    // Publish message
    mqtt_publish(&conn, NULL, topic, (uint8_t *)state_message, msg_len, 
                 MQTT_QOS_LEVEL_1, MQTT_RETAIN_OFF);
}

PROCESS_THREAD(mqtt_client_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    // Initialize MQTT connection
    mqtt_register(&conn, &mqtt_client_process, "client_id",
                  mqtt_event_callback, 256);
    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT,
                 1000, mqtt_event_callback, NULL);

    if (conn.state != MQTT_CONN_STATE_CONNECTED) {
        printf("Error: Unable to connect to MQTT broker\n");
        PROCESS_EXIT();
    }

    // Set a timer to publish the state every 5 seconds
    etimer_set(&timer, PUBLISH_INTERVAL);

    while (1) {
        PROCESS_YIELD();

        if (etimer_expired(&timer)) {
            publish_state();
            etimer_reset(&timer);
        }
    }

    PROCESS_END();
}

// A dummy callback for the MQTT events
void mqtt_event_callback(struct mqtt_connection *conn,
                         mqtt_event_t event, void *data) {
    printf("MQTT event occurred: %d\n", event);
}