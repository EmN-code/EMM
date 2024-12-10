
#include <stdio.h>
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "dev/dht11.h" // Ensure this header is available or replace with appropriate DHT11 driver

#define DHT11_GPIO_PIN    (1)
#define DHT11_GPIO_PORT   (0)
#define RGB_LED_PIN       (2) // Example pin for RGB LED
#define RGB_LED_PORT      (0)

#define RGB_LED_CYAN      (GPIO_HAL_PIN_VALUE_HIGH << RGB_LED_PIN)

PROCESS(dht11_rgb_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

static void read_dht11(void) {
    int temperature, humidity;
    if (dht11_read(DHT11_GPIO_PORT, DHT11_GPIO_PIN, &temperature, &humidity) == 0) {
        printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
    } else {
        printf("Failed to read DHT11 sensor\n");
    }
}

static void turn_on_rgb_led(void) {
    gpio_hal_set(RGB_LED_PORT, RGB_LED_PIN, GPIO_HAL_PIN_VALUE_HIGH);
}

PROCESS_THREAD(dht11_rgb_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    // Initialize GPIO pins
    gpio_hal_init();
    gpio_hal_dir(RGB_LED_PORT, RGB_LED_PIN, GPIO_HAL_DIR_OUTPUT);
    gpio_hal_dir(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_HAL_DIR_INPUT);

    etimer_set(&timer, CLOCK_SECOND * 5); // Set timer for 5 seconds

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        read_dht11();
        turn_on_rgb_led();

        etimer_reset(&timer);
    }

    PROCESS_END();
}
