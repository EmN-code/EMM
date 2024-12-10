
#include <stdio.h>
#include <xtimer.h>
#include "periph/gpio.h"  // Include the missing header file
#include "dht11.h"

#define DHT11_PIN GPIO_PIN(DHT11_CONFIGURE_GPIO_PORT, DHT11_CONFIGURE_GPIO_PIN)
#define RGB_LED_PIN GPIO_PIN(0, 0)  // Example GPIO pin for RGB LED

void read_dht11(void) {
    int16_t temp, hum;
    if (dht11_read(DHT11_PIN, &temp, &hum) == DHT11_STATUS_OKAY) {
        printf("Temperature: %d.%d C, Humidity: %d.%d %%\n",
               temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        printf("Failed to read DHT11 sensor\n");
    }
}

void toggle_rgb_led(void) {
    static int state = 0;
    gpio_write(RGB_LED_PIN, state);
    state = !state;
}

int main(void) {
    gpio_init(RGB_LED_PIN, GPIO_OUT);
    dht11_init(DHT11_PIN);

    while (1) {
        read_dht11();
        toggle_rgb_led();
        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}
