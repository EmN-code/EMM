
#include <stdio.h>
#include <thread.h>
#include <xtimer.h>
#include "dht11.h"
#include "periph/gpio.h"

#define DHT11_PIN GPIO_PIN(0, 15) // Replace with the actual GPIO pin connected to DHT11
#define RGB_LED_PIN GPIO_PIN(0, 16) // Replace with the actual GPIO pin connected to RGB LED

void read_dht11(void) {
    int16_t temperature, humidity;
    dht11_read(DHT11_PIN, &temperature, &humidity);
    printf("Temperature: %d.%d C, Humidity: %d.%d %%\n",
           temperature / 10, temperature % 10, humidity / 10, humidity % 10);
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
        xtimer_sleep(5);
    }

    return 0;
}
