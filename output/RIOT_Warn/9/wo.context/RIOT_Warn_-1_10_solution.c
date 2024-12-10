#include "xtimer.h"
#include "thread.h"
#include "dht.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(0, 0) // Example pin, replace with your DHT data pin
#define LED_PIN GPIO_PIN(0, 1) // Example pin, replace with your LED pin
#define TEMP_THRESHOLD 30

char stack[THREAD_STACKSIZE_MAIN]; // Allocate stack for the thread

void *temperature_thread(void *arg) {
    (void)arg;
    int16_t temperature = 0;
    int16_t humidity = 0;
    dht_params_t params;
    params.type = DHT_TYPE_DHT11;
    params.pin = DHT_PIN;

    // Initialize the DHT sensor
    if (dht_init(&params) != DHT_OK) {
        printf("Failed to initialize DHT11\n");
        return NULL;
    }

    while (1) {
        if (dht_read(&params, &humidity, &temperature) == DHT_OK) {
            printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);
            if (temperature > TEMP_THRESHOLD) {
                // Turn on LED
                gpio_set(LED_PIN);
            } else {
                // Turn off LED
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        xtimer_sleep(5); // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a thread to handle temperature monitoring
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, temperature_thread, NULL, "temp_thread");

    return 0;
}
