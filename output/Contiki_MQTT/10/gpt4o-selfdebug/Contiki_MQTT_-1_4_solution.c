#include <stdio.h>
#include "mqtt.h"
#include "contiki.h"

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

static struct mqtt_connection conn;
static char client_id[] = "nrf52840_client";
static char topic[] = "device/state";
static uint8_t payload[20];

PROCESS_THREAD(mqtt_example_process, ev, data)
{
    static struct etimer timer;
    PROCESS_BEGIN();

    // Initialize the MQTT connection
    mqtt_register(&conn, &mqtt_example_process, client_id, NULL, MQTT_MAX_SEGMENT_SIZE);

    // Connect to MQTT Broker
    mqtt_connect(&conn, "47.102.103.1", 1883, 60);
    
    // Set the timer for periodic publishing
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_YIELD();

        if(etimer_expired(&timer)) {
            // Publish the message
            snprintf((char *)payload, sizeof(payload), "work");
            mqtt_publish(&conn, NULL, topic, payload, strlen((char *)payload), MQTT_QOS_LEVEL_0);

            // Reset the timer
            etimer_reset(&timer);
        }
    }

    PROCESS_END();
}