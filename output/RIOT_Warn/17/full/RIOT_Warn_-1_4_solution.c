#include <periph/gpio.h>
#include <xtimer.h>
#include <stdio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(0, 5)  // Assuming GPIO port A5 for DHT11
#define LED_PIN GPIO_PIN(0, 3)  // Assuming GPIO port A3 for LED
#define THRESHOLD_TEMP 30  // Temperature threshold in degree Celsius

int main(void) {
    // Initialize GPIO
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error initializing GPIO for LED\n");
        return 1;
    }

    dht_t dev;
    int16_t temperature = 0, humidity = 0;

    // Initialize DHT sensor
    while (dht_init(&dev, DHT_PIN, DHT11) != 0) {
        printf("Failed to initialize DHT sensor, retrying...\n");
        xtimer_sleep(1);
    }

    printf("DHT initialized successfully\n");

    while (1) {
        // Read temperature and humidity
        int result = dht_read(&dev, &temperature, &humidity);
        if (result != 0) {
            printf("Error reading DHT sensor, code: %d\n", result);
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);

            // Check if temperature exceeds the threshold
            if (temperature / 10 > THRESHOLD_TEMP) {
                gpio_set(LED_PIN);
                printf("Temperature exceeded threshold! LED is ON\n");
            } else {
                gpio_clear(LED_PIN);
                printf("Temperature is under control. LED is OFF\n");
            }
        }
        xtimer_sleep(5);  // Wait for 5 seconds
    }
    return 0;
}