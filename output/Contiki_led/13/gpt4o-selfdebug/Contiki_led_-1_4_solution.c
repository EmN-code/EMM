#include net/routing/routing.h
#include net/netstack.h
#include net/mac/mac.h
#include dev/leds.h

#include <stdio.h>
#include "contiki.h"
#include "dht11.h"

// Avoid redefinition of UIP_CONF_TCP by proper configuration
#ifndef UIP_CONF_TCP
#define UIP_CONF_TCP 0
#endif

// Defining LED operations using RGB LED macros
#define RGB_LED_RED 0x01
#define RGB_LED_GREEN 0x02
#define RGB_LED_BLUE 0x04

#define RGB_LED_WHITE   (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)

static void rgb_led_on(void) {
    leds_on(RGB_LED_WHITE);
}

static void rgb_led_off(void) {
    leds_off(RGB_LED_WHITE);
}

PROCESS(dht11_process, "DHT11 process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data) {
    PROCESS_BEGIN();

    static struct etimer et;
    int temperature, humidity;

    while(1) {
        // Set timer for 5 seconds
        etimer_set(&et, 5 * CLOCK_SECOND);

        // Wait for the timer event
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Read DHT11 sensor data
        if (dht11_read(&temperature, &humidity) == DHT11_SUCCESS) {
            // Checking if reading is successful, operate the LED
            printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
            rgb_led_on();
        } else {
            printf("Failed to read DHT11 sensor\n");
            rgb_led_off();
        }
    }

    PROCESS_END();
}
