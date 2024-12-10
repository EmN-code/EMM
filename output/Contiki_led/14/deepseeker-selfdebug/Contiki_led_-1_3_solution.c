
#include <stdio.h>
#include <xtimer.h>
#include "periph/gpio.h"
#include "dht11.h"

#define DHT11_PIN GPIO_PIN(PORT_A, 0)  // Replace with your actual GPIO pin
#define RGB_LED_PIN GPIO_PIN(PORT_A, 1)  // Replace with your actual GPIO pin

#define INTERVAL (5 * 1000 * 1000U)  // 5 seconds in microseconds

void read_dht11(void) {
    int16_t temp, hum;
    if (dht11_read(DHT11_PIN, &temp, &hum) == DHT11_OK) {
        printf("Temperature: %d.%d C, Humidity: %d.%d %%\n",
               temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        puts("Failed to read from DHT11 sensor");
    }
}

void toggle_rgb_led(void) {
    static int state = 0;
    gpio_write(RGB_LED_PIN, state);
    state = !state;
}

int main(void) {
    // Initialize GPIO pins
    gpio_init(DHT11_PIN, GPIO_OUT);
    gpio_init(RGB_LED_PIN, GPIO_OUT);

    while (1) {
        read_dht11();
        toggle_rgb_led();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
