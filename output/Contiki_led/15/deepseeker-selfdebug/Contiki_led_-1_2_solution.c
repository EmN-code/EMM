
#include "contiki.h"
#include "dev/leds.h"
#include "dev/dht11.h"  // Ensure this header file is available
#include <stdio.h>

#define RGB_LED_RED    LEDS_RED
#define RGB_LED_GREEN  LEDS_GREEN
#define RGB_LED_BLUE   LEDS_BLUE
#define RGB_LED_CYAN   (RGB_LED_GREEN | RGB_LED_BLUE)

PROCESS(dht11_led_process, "DHT11 and LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

PROCESS_THREAD(dht11_led_process, ev, data)
{
    static struct etimer timer;
    static int humidity, temperature;

    PROCESS_BEGIN();

    // Initialize the DHT11 sensor
    dht11_init();

    // Set the timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor
        if (dht11_read(&humidity, &temperature) == DHT11_STATUS_OKAY) {
            printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);

            // Turn on the RGB LED (Cyan color)
            leds_set(RGB_LED_CYAN);
        } else {
            printf("Failed to read DHT11 sensor\n");
        }

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main() {
    // Start the Contiki system
    contiki_init();

    // Start the process
    process_start(&dht11_led_process, NULL);

    // Enter the Contiki main loop
    while(1) {
        process_run();
    }

    return 0;
}
