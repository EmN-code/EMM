
#include <stdio.h>
#include "xtimer.h"
#include "dht11.h"
#include "periph/gpio.h"

#define DHT11_PIN GPIO_PIN(DHT11_CONFIGURE_GPIO_PORT, DHT11_CONFIGURE_GPIO_PIN)
#define RGB_LED_PIN GPIO_PIN(0, 0) // Assuming RGB LED is connected to GPIO port 0, pin 0

void read_dht11(void) {
    int16_t temp, hum;
    if (dht11_read(DHT11_PIN, &temp, &hum) == DHT11_OK) {
        printf("Temperature: %d.%d C, Humidity: %d.%d %%\n", temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        printf("Failed to read DHT11 sensor\n");
    }
}

void turn_on_rgb_led(void) {
    gpio_set(RGB_LED_PIN);
}

int main(void) {
    // Initialize the DHT11 sensor
    dht11_init(DHT11_PIN);

    // Initialize the RGB LED
    gpio_init(RGB_LED_PIN, GPIO_OUT);

    while (1) {
        read_dht11();
        turn_on_rgb_led();
        xtimer_sleep(5); // Sleep for 5 seconds
    }

    return 0;
}
