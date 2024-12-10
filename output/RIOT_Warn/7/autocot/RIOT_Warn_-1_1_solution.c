#include <thread.h>
#include <stdio.h>
#include "xtimer.h"
#include "dht.h"
#include "periph/gpio.h"

#define INTERVAL (5U * US_PER_SEC) // 5 seconds interval
#define TEMP_THRESHOLD 30 // Temperature threshold in degree Celsius
#define DHT_PIN GPIO_PIN(PORT_A, 10) // Example pin for DHT11
#define LED_PIN GPIO_PIN(PORT_B, 5) // Example pin for LED

static dht_t dht_dev;
static int16_t temp, hum;

void *read_sensor(void *arg) {
    (void)arg;
    while (1) {
        if (dht_read(&dht_dev, &temp, &hum) != 0) {
            printf("Error reading DHT11\n");
        }
        else {
            printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum);
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Temperature threshold exceeded. LED ON.\n");
            }
            else {
                gpio_clear(LED_PIN);
                printf("Temperature below threshold. LED OFF.\n");
            }
        }
        xtimer_sleep(INTERVAL);
    }
    return NULL;
}

int main(void) {
    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN); // Turn off LED initially

    // Initialize DHT11 sensor
    if (dht_init(&dht_dev, DHT_TYPE_DHT11, DHT_PIN) != 0) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }

    // Create a thread to read the sensor values
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_WOUT_YIELD, read_sensor, NULL, "DHT Reader");

    // Keep the main function running
    while (1) {
        xtimer_sleep(INTERVAL);
    }

    return 0;
}
