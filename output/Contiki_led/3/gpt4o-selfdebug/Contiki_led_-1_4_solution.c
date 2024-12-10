#include "project-conf.h"
#include "contiki-net.h"
#include "contiki.h"
#include "simple-sensors/dht11.h"
#include "simple-leds/rgb-led.h"

#include <stdio.h>

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
    static struct etimer timer;
    static int temperature, humidity;
    
    PROCESS_BEGIN();

    // Initialize the DHT11 sensor and RGB LED
    dht11_init();
    rgb_led_init();

    // Set a timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        // Wait for the timer event
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read temperature and humidity from DHT11
        if(dht11_read(&temperature, &humidity) == DHT11_SUCCESS) {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);

            // Turn on LED (assuming a simple function to turn LED on)
            rgb_led_on();
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}