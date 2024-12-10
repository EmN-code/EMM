
#include "contiki.h"
#include "dev/leds.h"
#include "sys/etimer.h"
#include <stdio.h>

// Define the RGB LED colors
#define RGB_LED_WHITE   (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_YELLOW  (RGB_LED_RED | RGB_LED_GREEN)
#define RGB_LED_CYAN    (RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_MAGENTA (RGB_LED_RED | RGB_LED_BLUE)

// Define the DHT11 status
#define DHT11_STATUS_OKAY (0)

// Simulate DHT11 reading function
int dht11_read(void) {
    // Simulate a successful read
    return DHT11_STATUS_OKAY;
}

PROCESS(dht11_rgb_led_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_led_process);

PROCESS_THREAD(dht11_rgb_led_process, ev, data)
{
    static struct etimer timer;
    static int dht11_status;

    PROCESS_BEGIN();

    // Set the timer to 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        // Wait for the timer to expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor
        dht11_status = dht11_read();

        if(dht11_status == DHT11_STATUS_OKAY) {
            // Turn on the RGB LED in a specific color (e.g., WHITE)
            leds_set(RGB_LED_WHITE);
        } else {
            // Turn off the RGB LED if there's an error
            leds_off(LEDS_ALL);
        }

        // Reset the timer for the next reading
        etimer_reset(&timer);
    }

    PROCESS_END();
}
