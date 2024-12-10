// Ensure you have all necessary imports for the Contiki MQTT library.
#include "net/ipv6/uip-ds6.h"
#include "mqtt.h"
#include <stdio.h>
#include <string.h>

// Define some constants and state messages
#define BROKER_IP_ADDR          "47.102.103.1"
#define BROKER_PORT             1883
#define STATE_WORK              "work"
#define STATE_NOT_WORK          "not work"
#define PUBLISH_INTERVAL        (5 * CLOCK_SECOND)
#define MQTT_CLIENT_ID          "nrf52840_client"
#define MQTT_TOPIC              "device/state"

// Declare process
PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

static struct mqtt_connection conn;
static struct etimer pub_timer;

// Process thread implementation
PROCESS_THREAD(mqtt_publish_process, ev, data)
{
    static char pub_state[20];
    mqtt_status_t status;
    uint16_t packet_id;

    PROCESS_BEGIN();

    // Initialize mqtt connection
    mqtt_register(&conn, &mqtt_publish_process, MQTT_CLIENT_ID, mqtt_event_callback, 256);

    printf("Connecting to MQTT Broker at %s:%d\n", BROKER_IP_ADDR, BROKER_PORT);
    status = mqtt_connect(&conn, BROKER_IP_ADDR, BROKER_PORT, MQTT_KEEP_ALIVE_DEFAULT);

    if(status != MQTT_STATUS_OK) {
        printf("MQTT connection failed!\n");
        PROCESS_EXIT();
    }

    // Set a timer to manage publishing interval
    etimer_set(&pub_timer, PUBLISH_INTERVAL);

    while(1) {
        // Wait for timer event
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&pub_timer));

        // Toggle work state for demonstration
        strncpy(pub_state, (strcmp(pub_state, STATE_WORK) == 0) ? STATE_NOT_WORK : STATE_WORK, sizeof(pub_state));
        pub_state[sizeof(pub_state) - 1] = '\0';  // Ensure null-termination

        // Publish state to MQTT topic
        status = mqtt_publish(&conn, &packet_id, MQTT_TOPIC, (uint8_t *)pub_state, strlen(pub_state), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF, MQTT_TOPIC_ALIAS_NONE);

        if(status == MQTT_STATUS_OK) {
            printf("Published: %s\n", pub_state);
        } else {
            printf("Failed to publish message. Status = %d\n", status);
        }

        // Reset timer for next publish interval
        etimer_reset(&pub_timer);
    }

    PROCESS_END();
}

// Callback for handling events from the MQTT client
void mqtt_event_callback(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    switch (event) {
        case MQTT_EVENT_CONNECTED:
            printf("MQTT connection established.");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("MQTT connection disconnected.");
            break;
        default:
            printf("MQTT event: %d - Ignored
", event);
            break;
    }
}