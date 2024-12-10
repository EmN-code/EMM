
#include <stdio.h>
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "dev/dht11-sensor.h"

// Define the GPIO pins for DHT11 and RGB LED
#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN  1

#define RGB_LED_RED_PIN   2
#define RGB_LED_GREEN_PIN 3
#define RGB_LED_BLUE_PIN  4

// Define the RGB LED colors
#define RGB_LED_WHITE   (1 << RGB_LED_RED_PIN | 1 << RGB_LED_GREEN_PIN | 1 << RGB_LED_BLUE_PIN)
#define RGB_LED_MAGENTA (1 << RGB_LED_RED_PIN | 1 << RGB_LED_BLUE_PIN)
#define RGB_LED_YELLOW  (1 << RGB_LED_RED_PIN | 1 << RGB_LED_GREEN_PIN)
#define RGB_LED_CYAN    (1 << RGB_LED_GREEN_PIN | 1 << RGB_LED_BLUE_PIN)

// Function to initialize GPIO pins
void init_gpio(void) {
    gpio_hal_set_input(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
    gpio_hal_set_output(DHT11_GPIO_PORT, RGB_LED_RED_PIN);
    gpio_hal_set_output(DHT11_GPIO_PORT, RGB_LED_GREEN_PIN);
    gpio_hal_set_output(DHT11_GPIO_PORT, RGB_LED_BLUE_PIN);
}

// Function to turn on the RGB LED
void turn_on_rgb_led(uint32_t color) {
    gpio_hal_write(DHT11_GPIO_PORT, RGB_LED_RED_PIN, (color & (1 << RGB_LED_RED_PIN)) ? 1 : 0);
    gpio_hal_write(DHT11_GPIO_PORT, RGB_LED_GREEN_PIN, (color & (1 << RGB_LED_GREEN_PIN)) ? 1 : 0);
    gpio_hal_write(DHT11_GPIO_PORT, RGB_LED_BLUE_PIN, (color & (1 << RGB_LED_BLUE_PIN)) ? 1 : 0);
}

PROCESS(dht11_rgb_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

PROCESS_THREAD(dht11_rgb_process, ev, data) {
    static struct etimer timer;
    uint8_t humidity_int, humidity_dec, temperature_int, temperature_dec;

    PROCESS_BEGIN();

    init_gpio();

    while(1) {
        etimer_set(&timer, CLOCK_SECOND * 5); // Set timer for 5 seconds

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read DHT11 sensor
        if (dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER, &humidity_int) == DHT11_STATUS_OKAY &&
            dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL, &humidity_dec) == DHT11_STATUS_OKAY &&
            dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER, &temperature_int) == DHT11_STATUS_OKAY &&
            dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL, &temperature_dec) == DHT11_STATUS_OKAY) {

            printf("Humidity: %d.%d %%, Temperature: %d.%d C\n", humidity_int, humidity_dec, temperature_int, temperature_dec);

            // Turn on the RGB LED
            turn_on_rgb_led(RGB_LED_WHITE);
        } else {
            printf("Failed to read DHT11 sensor\n");
        }

        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void) {
    return 0;
}
