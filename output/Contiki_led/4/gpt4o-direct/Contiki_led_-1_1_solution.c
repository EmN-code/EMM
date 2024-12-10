#include "sys/etimer.h"
#include "sys/rtimer.h"
#include "sys/ctimer.h"
#include "sys/timer.h"
#include "dev/leds.h"
#include "dev/dht11.h"
#include "contiki.h"
#include <stdio.h>

// Process declaration
PROCESS(dht11_led_process, "DHT11 and LED Control Process");
AUTOSTART_PROCESSES(&dht11_led_process);

// Process implementation
PROCESS_THREAD(dht11_led_process, ev, data) {
    static struct etimer timer;
    static int temperature, humidity;

    PROCESS_BEGIN();

    while(1) {
        // Set the timer for 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);

        // Wait for the timer event
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read data from DHT11
        if(dht11_read(&temperature, &humidity) == DHT11_SUCCESS) {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);

            // Turn on LED
            leds_on(LEDS_RED);
            leds_on(LEDS_GREEN);
            leds_on(LEDS_BLUE);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }

        // Wait for a short period before turning off LEDs to simulate the turning on
        etimer_set(&timer, CLOCK_SECOND * 1);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Turn off LED
        leds_off(LEDS_RED);
        leds_off(LEDS_GREEN);
        leds_off(LEDS_BLUE);
    }

    PROCESS_END();
}
