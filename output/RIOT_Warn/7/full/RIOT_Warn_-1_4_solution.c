#include "periph/i2c.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "event/thread.h"
#include "MQTTClient.h"
#include "dht.h"
#include "thread.h"
#include "dht_params.h"

#define GPIO_LED (GPIO_NUM_2)  // GPIO number for LED
#define TEMP_THRESHOLD 30     // Temperature threshold in degree Celsius

static gpio_t led_pin = GPIO_LED;

dht_t dev;

void *thread_handler(void *arg) {
    (void)arg;

    int16_t temperature, humidity;
    while (1) {
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            puts("Error reading DHT sensor!");
        } else {
            printf("Temperature: %d *C, Humidity: %d %%\n", temperature, humidity);
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(led_pin); // Turn on the LED
            } else {
                gpio_clear(led_pin); // Turn off the LED
            }
        }
        ztimer_sleep(ZTIMER_MSEC, 5000); // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    if (gpio_init(led_pin, GPIO_OUT) != ESP_OK) {
        puts("Failed to initialize GPIO pin for LED!");
        return 1;
    }

    // Initialize DHT sensor
    if (dht_init(&dev, &dht_params[0], 0) != ESP_OK) {
        puts("Failed to initialize DHT sensor!");
        return 1;
    }

    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, thread_handler, NULL, "dht_thread");

    return 0;
}