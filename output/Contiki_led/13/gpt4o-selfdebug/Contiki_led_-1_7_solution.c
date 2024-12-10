#include "contiki.h"
#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki-default-conf.h"
#include <stdio.h>

#define DHT11_STATUS_CHECKSUM_FAILED (2)
#define BUTTON_HAL_DEBOUNCE_DURATION (CLOCK_SECOND >> 6)
#define RGB_LED_RED (1 << 0)
#define RGB_LED_GREEN (1 << 1)
#define RGB_LED_BLUE (1 << 2)
#define RGB_LED_YELLOW (RGB_LED_RED | RGB_LED_GREEN)

PROCESS(dht11_process, "DHT11 Reader");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    // Set timer to expire every 5 seconds
    etimer_set(&timer, 5 * CLOCK_SECOND);

    while(1) {
        // Wait for the timer to expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        printf("Reading DHT11 sensor...\n");

        // Simulate reading DHT11 data since actual library is not available
        int humidity = 60; // Placeholder value
        int temperature = 22; // Placeholder value

        printf("Temperature: %d °C, Humidity: %d %%\n", temperature, humidity);

        // Turn on the RGB LED based on temperature
        if (temperature > 25) {
            printf("Turning on RGB LED: RED\n");
            // Turn on RED LED code goes here
        } else {
            printf("Turning on RGB LED: GREEN\n");
            // Turn on GREEN LED code goes here
        }

        // Reset the timer to expire again in 5 seconds
        etimer_reset(&timer);
    }

    PROCESS_END();
}
