// Includes based on descending alphabetical order and necessary libraries
#include <stdio.h>
#include <stdint.h>
#include "contiki.h"
#include "dev/dht11.h"
#include "dev/rgb-led.h"

// Define LED color based on documentation
#define LED_COLOR RGB_LED_YELLOW

PROCESS(dht11_reader_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&dht11_reader_process);

PROCESS_THREAD(dht11_reader_process, ev, data)
{
    static struct etimer timer;
    int16_t temperature, humidity;

    PROCESS_BEGIN();

    // Configure and start timer to execute every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read values from DHT11 sensor
        if (dht11_read(&temperature, &humidity) != DHT11_ERROR) {
            printf("Temperature: %d 'C, Humidity: %d %%\n", temperature, humidity);

            // Turn on the RGB LED with the defined color
            rgb_led_on(LED_COLOR);
        } else {
            printf("Failed to read data from DHT11\n");
        }

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void) {
    contiki_init();
    process_start(&dht11_reader_process, NULL);
    while (1) {
        // Run the main loop
        process_run();
    }
    return 0;
}