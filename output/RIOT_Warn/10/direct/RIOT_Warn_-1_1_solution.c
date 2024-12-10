#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"
#include "stdio.h"

#define LED_PIN GPIO_PIN(PORT_A, 6) // Define the LED pin
#define TEMP_THRESHOLD 30 // Temperature threshold in degree Celsius
#define DHT_PIN GPIO_PIN(PORT_B, 5) // DHT11 data pin

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;

    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        puts("Error initializing DHT11 sensor\n");
        return 1;
    }

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            puts("Error reading from DHT11 sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                    temperature / 10, temperature % 10,
                    humidity / 10, humidity % 10);
            
            // Check temperature and control LED
            if (temperature / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                puts("Warning: Temperature threshold exceeded!\n");
            } else {
                gpio_clear(LED_PIN);
            }
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}