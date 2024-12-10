
#include "contiki.h"
#include "sys/clock.h"
#include "sys/etimer.h"
#include "dev/gpio-hal.h"
#include "dev/dht11-sensor.h"

#define LED_PIN 23  // Example GPIO pin for the RGB LED

PROCESS(dht11_led_process, "DHT11 and LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

PROCESS_THREAD(dht11_led_process, ev, data)
{
    static struct etimer et;
    static uint8_t humidity, temperature;

    PROCESS_BEGIN();

    // Initialize GPIO for the LED
    gpio_hal_init();
    gpio_hal_output(LED_PIN);

    while(1) {
        // Set the etimer to trigger every 5 seconds
        etimer_set(&et, CLOCK_SECOND * 5);

        // Wait for the timer to expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Read DHT11 sensor values
        if(dht11_read(&humidity, &temperature) == DHT11_STATUS_OKAY) {
            // Turn on the RGB LED
            gpio_hal_set(LED_PIN);
        } else {
            // Turn off the RGB LED in case of error
            gpio_hal_clear(LED_PIN);
        }

        // Reset the timer for the next cycle
        etimer_reset(&et);
    }

    PROCESS_END();
}
