#include "contiki.h"
#include "dev/leds.h"
#include "dev/xyz_led.h" // Assume this is the correct header for RGB_LED_MAGENTA
#include <stdio.h>

PROCESS(dht11_reader_process, "DHT11 Reader");
AUTOSTART_PROCESSES(&dht11_reader_process);

static struct etimer et;

PROCESS_THREAD(dht11_reader_process, ev, data)
{
    PROCESS_BEGIN();

    // Initialize the etimer to trigger every 5 seconds
    etimer_set(&et, CLOCK_SECOND * 5);

    while(1) {
        // Wait for the timer event to occur
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        printf("Reading DHT11 sensor\n");

        // Simulate reading DHT11 sensor data (temperature and humidity)
        int temperature = 25; // Placeholder value
        int humidity = 60; // Placeholder value

        printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);

        printf("Turning on RGB LED with magenta color\n");
        rgb_led_on(RGB_LED_MAGENTA); // Turn on the LED

        etimer_reset(&et); // Reset the etimer for the next 5 seconds
    }

    PROCESS_END();
}

void rgb_led_on(int color) {
    leds_on(color);
}