
#include <stdio.h>
#include <stdlib.h>
#include "thread.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht11.h" // Include the correct header for DHT11 sensor

#define DHT11_PIN GPIO_PIN(0, 15) // Example GPIO pin for DHT11 sensor
#define RGB_LED_PIN GPIO_PIN(0, 16) // Example GPIO pin for RGB LED

#define SLEEP_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void *dht11_thread(void *arg)
{
    (void)arg;

    int16_t temp, hum;
    while (1) {
        if (dht11_read(DHT11_PIN, &hum, &temp) == DHT11_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            gpio_set(RGB_LED_PIN); // Turn on the RGB LED
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }
        xtimer_usleep(SLEEP_INTERVAL);
    }
    return NULL;
}

int main(void)
{
    if (gpio_init(RGB_LED_PIN, GPIO_OUT) < 0) {
        printf("Failed to initialize RGB LED GPIO\n");
        return 1;
    }

    thread_create(NULL, THREAD_STACKSIZE, THREAD_PRIORITY_MAIN - 1, 0, dht11_thread, NULL, "dht11_thread");

    return 0;
}
