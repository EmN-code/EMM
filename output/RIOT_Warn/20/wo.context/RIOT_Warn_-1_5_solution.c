#include <xtimer.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(PORT_A, 5)  // Set this to a valid pin number
#define LED_PIN GPIO_PIN(PORT_A, 10) // Set this to a valid pin number
#define TEMP_THRESHOLD 30

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        printf("[Error] Could not initialize DHT sensor\n");
        return 1;
    }

    while (1) {
        // Read the DHT sensor
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
            printf("[Error] Failed to read from DHT sensor\n");
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}