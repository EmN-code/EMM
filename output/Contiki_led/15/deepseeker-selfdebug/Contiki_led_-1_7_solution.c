
#include <stdio.h>
#include <xtimer.h>
#include "periph/gpio.h"
#include "dht11.h"

#define DHT11_PIN GPIO_PIN(PORT_A, 0)  // Replace with your actual DHT11 pin
#define RGB_LED_PIN GPIO_PIN(PORT_A, 1)  // Replace with your actual RGB LED pin

#define RGB_LED_RED   0x01
#define RGB_LED_GREEN 0x02
#define RGB_LED_BLUE  0x04

#define RGB_LED_WHITE   (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_MAGENTA (RGB_LED_RED | RGB_LED_BLUE)
#define RGB_LED_YELLOW  (RGB_LED_RED | RGB_LED_GREEN)
#define RGB_LED_CYAN    (RGB_LED_GREEN | RGB_LED_BLUE)

static dht11_t dht11;

void read_dht11(void) {
    int16_t temp, hum;
    if (dht11_read(&dht11, DHT11_PIN, &temp, &hum) == DHT11_OK) {
        printf("Temperature: %d.%d C, Humidity: %d%%\n", temp / 10, temp % 10, hum);
        // Turn on the RGB LED based on the temperature
        if (temp < 200) {  // Assuming temperature is in tenths of a degree
            gpio_set(RGB_LED_PIN);  // Turn on the LED
        } else {
            gpio_clear(RGB_LED_PIN);  // Turn off the LED
        }
    } else {
        puts("Failed to read from DHT11 sensor");
    }
}

int main(void) {
    // Initialize the DHT11 sensor
    dht11_init(&dht11);

    // Initialize the RGB LED pin
    gpio_init(RGB_LED_PIN, GPIO_OUT);

    while (1) {
        read_dht11();
        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}
