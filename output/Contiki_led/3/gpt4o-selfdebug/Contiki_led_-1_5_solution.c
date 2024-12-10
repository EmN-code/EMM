#include <zephyr/sys/fdtable.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/tracing/tracing.h>

#include "contiki.h"
#include "dev/dht11.h" // Include the correct header file for DHT11 sensor
#include "dev/rgb_led.h" // Include the header file for RGB LED

#define LOOP_INTERVAL (CLOCK_SECOND * 5) // 5 seconds

PROCESS(read_dht11_process, "DHT11 Reading and RGB LED Control");
AUTOSTART_PROCESSES(&read_dht11_process);

PROCESS_THREAD(read_dht11_process, ev, data)
{
    static struct etimer timer;

    PROCESS_BEGIN();

    // Initialize the sensor and LED
    dht11_init();
    rgb_led_init();

    while(1) {
        // Set the timer to fire every 5 seconds
        etimer_set(&timer, LOOP_INTERVAL);

        // Wait for timer event
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read temperature and humidity from DHT11
        int temperature, humidity;
        if (dht11_read(&temperature, &humidity) == 0) {
            // Successfully read the sensor, now turn on the RGB LED
            rgb_led_set(0, 255, 0); // Green color
        } else {
            // Reading failed, set the RGB LED to red color
            rgb_led_set(255, 0, 0);
        }

        // Allow the RGB LED to stay on for a short period
        etimer_set(&timer, CLOCK_SECOND / 2);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        
        // Turn off the RGB LED
        rgb_led_set(0, 0, 0);
    }

    PROCESS_END();
}