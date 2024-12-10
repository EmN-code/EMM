
#include <stdio.h>
#include <stdlib.h>
#include "thread.h" // Include the missing header file
#include "periph/gpio.h"
#include "xtimer.h"
#include "dht11.h"

#define DHT11_GPIO_PIN GPIO_PIN(0, 1)
#define RGB_LED_RED GPIO_PIN(0, 2)
#define RGB_LED_GREEN GPIO_PIN(0, 3)
#define RGB_LED_BLUE GPIO_PIN(0, 4)

#define RGB_LED_WHITE   (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_MAGENTA (RGB_LED_RED | RGB_LED_BLUE)
#define RGB_LED_YELLOW  (RGB_LED_RED | RGB_LED_GREEN)
#define RGB_LED_CYAN    (RGB_LED_GREEN | RGB_LED_BLUE)

static void *dht11_read_thread(void *arg) {
    (void)arg;
    dht11_t dev;
    int16_t temp, hum;

    if (dht11_init(&dev, DHT11_GPIO_PIN) == DHT11_OK) {
        while (1) {
            if (dht11_read(&dev, &temp, &hum) == DHT11_OK) {
                printf("Temperature: %d.%d C, Humidity: %d.%d %%\n",
                       temp / 10, temp % 10, hum / 10, hum % 10);
                gpio_set(RGB_LED_RED);
                xtimer_sleep(1);
                gpio_clear(RGB_LED_RED);
            } else {
                printf("Failed to read from DHT11 sensor\n");
            }
            xtimer_sleep(5);
        }
    } else {
        printf("Failed to initialize DHT11 sensor\n");
    }

    return NULL;
}

int main(void) {
    gpio_init(RGB_LED_RED, GPIO_OUT);
    gpio_init(RGB_LED_GREEN, GPIO_OUT);
    gpio_init(RGB_LED_BLUE, GPIO_OUT);

    thread_create(NULL, 2048, THREAD_PRIORITY_MAIN - 1, 0, dht11_read_thread, NULL, "dht11_read");

    return 0;
}
