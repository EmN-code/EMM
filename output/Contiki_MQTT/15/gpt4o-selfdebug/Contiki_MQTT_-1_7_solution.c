#include <stdint.h>
#include "mqtt.h"
#include "simple-udp.h"
#include "contiki.h"

#define UDP_PORT 1234

PROCESS(mqtt_publisher_process, "MQTT Publisher");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

static struct etimer timer;
static struct mqtt_connection conn;

PROCESS_THREAD(mqtt_publisher_process, ev, data) {
    PROCESS_BEGIN();

    char *mqtt_broker_host = "47.102.103.1";
    uint16_t mqtt_broker_port = 1883;
    char *client_id = "nrf52840_client";
    char *topic = "device/state";
    char *message = "work or not work";
    uint16_t msg_id;
    mqtt_status_t status;

    // Connect to the MQTT broker
    status = mqtt_connect(&conn, mqtt_broker_host, mqtt_broker_port, 60, 1, NULL);
    if (status != MQTT_STATUS_OK) {
        printf("Failed to connect to broker\n");
        PROCESS_EXIT();
    }

    // Define the timer
    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);

        // Publish the message
        status = mqtt_publish(&conn, &msg_id, topic, (uint8_t *)message,
                              strlen(message), 0);
                              
        if (status != MQTT_STATUS_OK) {
            printf("Failed to publish message\n");
        }

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}