#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 4) // Change this to your specific GPIO pin
#define LED_PIN GPIO_PIN(0, 2) // Change this to your specific GPIO pin
#define DHT_TYPE DHT11        // DHT11 or DHT22
#define TEMP_THRESHOLD 30     // Temperature threshold for turning on the LED

void *sensor_thread(void *arg) {
    (void)arg;
    dht_t dev;

    // Initialize the DHT sensor
    if (dht_init(&dev, &gpio_dev(DHT_PIN)) != DHT_OK) {
        printf("Error initializing DHT sensor\n");
        return NULL;
    }

    while (1) {
        int16_t temp, hum;
        int result = dht_read(&dev, &temp, &hum);

        if (result == DHT_OK) {
            printf("Temperature: %d.%d°C\n", temp / 10, temp % 10);
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Error reading from DHT sensor\n");
        }

        xtimer_sleep(5); // Sleep for 5 seconds
    }

    return NULL;
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, 0, sensor_thread, NULL, "sensor");
    return 0;
}