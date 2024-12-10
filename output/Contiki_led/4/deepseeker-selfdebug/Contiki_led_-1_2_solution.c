
#include "contiki.h"
#include "dev/leds.h"
#include "dev/dht11.h"  // Ensure this header is available in your Contiki environment

#include <stdio.h>

#define RGB_LED_RED    (1 << 0)
#define RGB_LED_GREEN  (1 << 1)
#define RGB_LED_BLUE   (1 << 2)
#define RGB_LED_CYAN   (RGB_LED_GREEN | RGB_LED_BLUE)

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
    static struct etimer timer;
    static int humidity, temperature;

    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 5);  // Set timer to 5 seconds

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read DHT11 sensor
        if (dht11_read(&humidity, &temperature) == DHT11_STATUS_OKAY) {
            printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);
            leds_set(RGB_LED_CYAN);  // Turn on RGB LED
        } else {
            printf("Failed to read DHT11 sensor\n");
            leds_off(RGB_LED_CYAN);  // Turn off RGB LED
        }

        etimer_reset(&timer);  // Reset the timer
    }

    PROCESS_END();
}

int main(void)
{
    // Initialize Contiki
    contiki_init();

    // Start the process
    process_start(&dht11_read_process, NULL);

    // Enter the Contiki main loop
    while(1) {
        process_run();
    }

    return 0;
}
