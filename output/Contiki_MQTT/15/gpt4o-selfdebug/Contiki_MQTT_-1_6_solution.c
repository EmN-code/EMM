#include "simple-udp.h"
#include "mqtt.h"
#include "contiki.h"

// Define constants
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define UPDATE_INTERVAL (5 * CLOCK_SECOND)

// Function to handle MQTT events
static void mqtt_event_handler(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    switch(event) {
        case MQTT_EVENT_CONNECTED:
            printf("Connected to MQTT broker\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("Disconnected from MQTT broker\n");
            break;
        // Handle other MQTT events if necessary
        default:
            break;
    }
}

// Process definition
PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

PROCESS_THREAD(mqtt_publisher_process, ev, data) {
    static struct etimer timer;
    static struct mqtt_connection conn;
    static uint16_t mid;
    static char *state_msg = "work";

    PROCESS_BEGIN();

    // Register the MQTT connection
    mqtt_register(&conn, &mqtt_publisher_process, "my_mqtt_client", mqtt_event_handler, 
                  MQTT_MAX_PACKET_SIZE);

    // Connect to the MQTT broker
    mqtt_status_t status = mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60, MQTT_CLEAN_SESSION_ON, NULL);
    if(status != MQTT_STATUS_OK) {
        printf("Could not connect to MQTT broker: %d\n", status);
        PROCESS_EXIT();
    }

    // Set a timer to publish messages periodically
    etimer_set(&timer, UPDATE_INTERVAL);

    while(1) {
        PROCESS_YIELD();

        if(ev == PROCESS_EVENT_TIMER) {
            // Publish the state message
            status = mqtt_publish(&conn, &mid, "state_topic", (uint8_t *)state_msg, strlen(state_msg), 
                                  MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF, 0, MQTT_TOPIC_ALIAS_DISABLED, NULL);
            if(status == MQTT_STATUS_OK) {
                printf("Published state message: %s\n", state_msg);
            } else {
                printf("Failed to publish state message: %d\n", status);
            }

            // Restart the timer
            etimer_reset(&timer);
        }
    }

    PROCESS_END();
}