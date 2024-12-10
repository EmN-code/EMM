#import necessary headers in descending alphabetical order
#include <gpio.h>
#include <xtimer.h>
#include <stdio.h>
#include <thread.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(PORT_A, 1)
#define LED_PIN GPIO_PIN(PORT_B, 2)
#define TEMP_THRESHOLD 30 // Temperature threshold for LED warning

void *temperature_monitor(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temperature, humidity;
    
    // Initialize the DHT sensor
    dht_init(&dev, DHT_PIN, DHT11);
    
    while (1) {
        // Read values from DHT11 sensor
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                   temperature / 10, temperature % 10, 
                   humidity / 10, humidity % 10);

            // Check if temperature exceeds threshold
            if (temperature / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Error reading DHT11 sensor\n");
        }

        // Wait for 5 seconds before next read
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    // Initialize GPIO pins for LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Start the temperature monitor thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, temperature_monitor, NULL, "temp_monitor");

    return 0;
}