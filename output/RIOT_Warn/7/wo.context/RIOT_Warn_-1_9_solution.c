#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(PORT_B, 5)  // Define the correct GPIO port and pin
#define LED_PIN GPIO_PIN(PORT_A, 10) // Define the correct GPIO port and pin
#define TEMP_THRESHOLD 30            // Temperature threshold

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;

    // Initialize DHT11
    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        puts("DHT init failed");
        return 1;
    }

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        // Read DHT11 sensor
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);

            // Check if temperature exceeds threshold
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            puts("DHT read failed");
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }
    return 0;
}