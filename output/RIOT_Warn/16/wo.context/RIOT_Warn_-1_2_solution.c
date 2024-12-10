#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(PORT_A, 5) // Pin where the DHT11 is connected
#define LED_PIN GPIO_PIN(PORT_B, 0) // Pin where the LED is connected
#define TEMP_THRESHOLD 30 // Temperature threshold in degree Celsius
#define SLEEP_INTERVAL (5U * 1000U * 1000U) // 5 seconds in microseconds

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    // Initialize DHT device
    if (dht_init(&dev, DHT_PIN, DHT11) != 0) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        // Read the sensor data
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read DHT11 sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                    temp / 10, temp % 10, hum / 10, hum % 10);

            // Check temperature and control LED
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Temperature is above threshold! LED ON\n");
            } else {
                gpio_clear(LED_PIN);
                printf("Temperature is below threshold! LED OFF\n");
            }
        }

        // Sleep for 5 seconds
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}