#include "xtimer.h"
#include "thread.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN         GPIO_PIN(0, 4)    // DHT11 data pin
#define LED_PIN         GPIO_PIN(0, 2)    // LED pin
#define TEMP_THRESHOLD  30                // Temperature threshold in Celsius
#define INTERVAL        (5U * US_PER_SEC) // 5 seconds interval

static dht_t dev;

static void *sensor_thread(void *arg) {
    (void)arg;
    int16_t temp, hum;

    // Initialize the DHT sensor
    if (dht_init(&dev, &(dht_params_t){.pin = DHT_PIN, .type = DHT11}) != 0) {
        puts("Failed to initialize DHT11 sensor\n");
        return NULL;
    }

    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    while (1) {
        // Read the DHT sensor
        if (dht_read(&dev, &temp, &hum) != 0) {
            puts("Failed to read from DHT11 sensor\n");
        } else {
            printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum);

            // Check temperature threshold
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn on LED
            } else {
                gpio_clear(LED_PIN); // Turn off LED
            }
        }

        // Sleep for the defined interval
        xtimer_usleep(INTERVAL);
    }

    return NULL;
}

int main(void) {
    // Create a sensor thread
    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, sensor_thread, NULL, "sensor_thread");

    return 0;
}