#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 6)  // Example pin, adjust as necessary
#define LED_PIN GPIO_PIN(PORT_C, 5)  // Example pin, adjust as necessary
#define DHT_TYPE DHT11
#define TEMP_THRESHOLD 30 // Threshold temperature in Celsius

int main(void) {
    // Initialize GPIO for LED
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    // Initialize DHT
    dht_t dev;
    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        printf("Error initializing DHT sensor\n");
        return 1;
    }

    int16_t temperature, humidity;

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN); // Turn on LED
            } else {
                gpio_clear(LED_PIN); // Turn off LED
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        xtimer_sleep(5); // Sleep for 5 seconds
    }

    return 0;
}