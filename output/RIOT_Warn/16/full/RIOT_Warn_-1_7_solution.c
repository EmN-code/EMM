#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <periph/dht.h>

#define TEMP_THRESHOLD 25
#define DHT_PIN GPIO_PIN(PORT_A, 0) // Example pin, adjust for your setup
#define LED_PIN GPIO_PIN(PORT_B, 0) // Example pin, adjust for your setup

int main(void) {
    int16_t temp, hum;
    dht_t dev;
    
    // Initialize DHT sensor
    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        printf("DHT initialization failed\n");
        return 1;
    }
    
    // Initialize LED pin
    if (gpio_init(LED_PIN, GPIO_OUT) != 0) {
        printf("Failed to initialize LED pin\n");
        return 1;
    }

    while (1) {
        // Read temperature and humidity
        while (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Error reading DHT sensor\n");
            xtimer_sleep(1);  // Retry every 1 second if reading fails
        }

        // Convert temperature to Celsius
        float temperature = temp / 10.0;
        printf("Temperature: %.1f°C\n", temperature);

        // LED control based on temperature threshold
        if (temperature > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
            printf("Temperature warning: LED ON\n");
        } else {
            gpio_clear(LED_PIN);
            printf("Temperature normal: LED OFF\n");
        }

        // Wait for 5 seconds before next reading
        xtimer_sleep(5);
    }
    return 0;
}