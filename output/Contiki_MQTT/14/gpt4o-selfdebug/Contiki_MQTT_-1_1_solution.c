#include <contiki.h>
#include <stdio.h>
#include <string.h>

// MQTT configuration
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUB_TOPIC "device/state"
#define MQTT_CLIENT_ID "nrf52840-client"

static struct etimer periodic_timer;

PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

PROCESS_THREAD(mqtt_publisher_process, ev, data)
{
    static bool state = false;
    PROCESS_BEGIN();

    // set the timer to fire every 5 seconds
    etimer_set(&periodic_timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

        // Toggle the state
        state = !state;

        // Publish the state to MQTT broker
        if (state) {
            printf("Publishing: work\n");
            // Call the function to publish 'work' to the MQTT broker
        } else {
            printf("Publishing: not work\n");
            // Call the function to publish 'not work' to the MQTT broker
        }

        // Reset the timer
        etimer_reset(&periodic_timer);
    }

    PROCESS_END();
}
