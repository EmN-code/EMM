
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

PROCESS(dht11_rgb_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

static void read_dht11(void) {
    uint8_t humidity_integer, humidity_decimal, temperature_integer, temperature_decimal;
    int status = dht11_read(&humidity_integer, &humidity_decimal, &temperature_integer, &temperature_decimal);

    if (status == DHT11_STATUS_OKAY) {
        printf("Humidity: %d.%d %%, Temperature: %d.%d C\n", humidity_integer, humidity_decimal, temperature_integer, temperature_decimal);
        // Turn on the RGB LED based on temperature
        if (temperature_integer < 20) {
            leds_set(RGB_LED_BLUE);
        } else if (temperature_integer < 30) {
            leds_set(RGB_LED_GREEN);
        } else {
            leds_set(RGB_LED_RED);
        }
    } else {
        printf("Failed to read DHT11 sensor\n");
    }
}

PROCESS_THREAD(dht11_rgb_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 5); // Set timer to 5 seconds

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        read_dht11();
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void) {
    process_start(&dht11_rgb_process, NULL);
    return 0;
}
