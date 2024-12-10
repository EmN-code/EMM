
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "contiki.h"
#include "dev/dht11.h" // Ensure this header is available in your Contiki environment
#include "dev/leds.h"

#define RGB_LED_RED   0x01
#define RGB_LED_GREEN 0x02
#define RGB_LED_BLUE  0x04

#define RGB_LED_WHITE   (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_MAGENTA (RGB_LED_RED | RGB_LED_BLUE)
#define RGB_LED_YELLOW  (RGB_LED_RED | RGB_LED_GREEN)
#define RGB_LED_CYAN    (RGB_LED_GREEN | RGB_LED_BLUE)

PROCESS(dht11_rgb_led_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_led_process);

void rgb_led_set(uint8_t colour) {
    leds_set(colour);
}

PROCESS_THREAD(dht11_rgb_led_process, ev, data) {
    static struct etimer timer;
    uint8_t humidity_int, humidity_dec, temperature_int, temperature_dec;
    uint8_t status;

    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 5); // Set timer for 5 seconds

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        status = dht11_read(&humidity_int, &humidity_dec, &temperature_int, &temperature_dec);

        if (status == DHT11_STATUS_OKAY) {
            printf("Humidity: %d.%d %% Temperature: %d.%d C\n", humidity_int, humidity_dec, temperature_int, temperature_dec);
            rgb_led_set(RGB_LED_WHITE); // Turn on RGB LED
        } else {
            printf("Failed to read DHT11 sensor\n");
            rgb_led_set(RGB_LED_RED); // Turn on RGB LED in red to indicate error
        }

        etimer_reset(&timer);
    }

    PROCESS_END();
}
