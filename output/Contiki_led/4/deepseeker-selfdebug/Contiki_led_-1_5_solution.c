
#include "contiki.h"
#include "dev/dht11.h" // Ensure this header file is available in your Contiki environment
#include "dev/leds.h"
#include <stdio.h>

#define RGB_LED_RED   0x01
#define RGB_LED_GREEN 0x02
#define RGB_LED_BLUE  0x04

#define RGB_LED_WHITE  (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_YELLOW (RGB_LED_RED | RGB_LED_GREEN)
#define RGB_LED_CYAN   (RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_MAGENTA (RGB_LED_RED | RGB_LED_BLUE)

PROCESS(dht11_rgb_led_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_led_process);

static void rgb_led_set(uint8_t colour) {
    leds_set(colour);
}

PROCESS_THREAD(dht11_rgb_led_process, ev, data) {
    static struct etimer timer;
    static uint8_t humidity_integer, temperature_integer;
    static uint8_t humidity_decimal, temperature_decimal;
    static uint8_t status;

    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 5); // Set timer for 5 seconds

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read DHT11 sensor values
        status = dht11_sensor.value(&dht11_sensor, DHT11_VALUE_HUMIDITY_INTEGER, &humidity_integer);
        if (status != DHT11_STATUS_OKAY) {
            printf("Failed to read humidity integer part\n");
            continue;
        }

        status = dht11_sensor.value(&dht11_sensor, DHT11_VALUE_TEMPERATURE_INTEGER, &temperature_integer);
        if (status != DHT11_STATUS_OKAY) {
            printf("Failed to read temperature integer part\n");
            continue;
        }

        // Turn on RGB LED based on sensor readings
        if (temperature_integer > 25) {
            rgb_led_set(RGB_LED_RED);
        } else if (temperature_integer < 20) {
            rgb_led_set(RGB_LED_BLUE);
        } else {
            rgb_led_set(RGB_LED_GREEN);
        }

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}
