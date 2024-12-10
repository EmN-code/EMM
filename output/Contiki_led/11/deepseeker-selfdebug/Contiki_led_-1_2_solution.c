
#include "contiki.h"
#include "dev/dht11.h"  // Ensure this header file is included
#include "dev/leds.h"
#include <stdio.h>

#define RGB_LED_RED    (LEDS_RED)
#define RGB_LED_GREEN  (LEDS_GREEN)
#define RGB_LED_BLUE   (LEDS_BLUE)

#define RGB_LED_YELLOW  (RGB_LED_RED | RGB_LED_GREEN)
#define RGB_LED_CYAN    (RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_MAGENTA (RGB_LED_RED | RGB_LED_BLUE)
#define RGB_LED_WHITE   (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)

#define DHT11_VALUE_HUMIDITY_INTEGER       (0)
#define DHT11_VALUE_HUMIDITY_DECIMAL       (1)
#define DHT11_VALUE_TEMPERATURE_DECIMAL    (3)

PROCESS(dht11_rgb_led_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_led_process);

PROCESS_THREAD(dht11_rgb_led_process, ev, data)
{
    static struct etimer timer;
    uint8_t dht11_data[4];

    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 5);  // Set timer to 5 seconds

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        if(dht11_read(dht11_data) == 0) {
            printf("Humidity: %d.%d %% Temperature: %d.%d C\n",
                   dht11_data[DHT11_VALUE_HUMIDITY_INTEGER],
                   dht11_data[DHT11_VALUE_HUMIDITY_DECIMAL],
                   dht11_data[DHT11_VALUE_TEMPERATURE_DECIMAL],
                   dht11_data[DHT11_VALUE_TEMPERATURE_DECIMAL]);

            // Turn on the RGB LED based on the temperature reading
            if(dht11_data[DHT11_VALUE_TEMPERATURE_DECIMAL] < 10) {
                leds_set(RGB_LED_BLUE);
            } else if(dht11_data[DHT11_VALUE_TEMPERATURE_DECIMAL] < 20) {
                leds_set(RGB_LED_GREEN);
            } else if(dht11_data[DHT11_VALUE_TEMPERATURE_DECIMAL] < 30) {
                leds_set(RGB_LED_YELLOW);
            } else {
                leds_set(RGB_LED_RED);
            }
        } else {
            printf("Failed to read DHT11 sensor\n");
        }

        etimer_reset(&timer);
    }

    PROCESS_END();
}
