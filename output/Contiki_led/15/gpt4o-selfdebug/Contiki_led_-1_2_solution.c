#include "netstack_ip_action.h"
#include "dev/leds.h"
#include "dev/dht11.h"
#include "etimer.h"
#include <stdio.h>

PROCESS(dht11_process, "DHT11 Example Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
    static struct etimer timer;
    static uint8_t temperature, humidity;

    PROCESS_BEGIN();

    // Set the timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Reset the timer
        etimer_reset(&timer);

        // Read the sensor
        if (dht11_read(&temperature, &humidity) == DHT11_STATUS_CHECKSUM_SUCCESS) {
            printf("Temperature: %d Humidity: %d\n", temperature, humidity);

            // Turn on the RGB LED according to temperature
            if (temperature > 25) {
                // If temperature is greater than 25, turn on RED
                leds_on(RGB_LED_RED);
            } else {
                // Otherwise turn on GREEN
                leds_on(RGB_LED_GREEN);
            }
        } else {
            printf("Failed to read from DHT11 sensor!\n");
        }
    }

    PROCESS_END();
}