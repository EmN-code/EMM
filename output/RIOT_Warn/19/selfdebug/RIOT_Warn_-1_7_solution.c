#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN     GPIO_PIN(0, 4)  // DHT sensor connected to GPIO4
#define LED_PIN     GPIO_PIN(0, 2)  // LED connected to GPIO2
#define TEMP_THRESHOLD 30           // Temperature threshold in degree Celsius
#define DELAY       (5U * US_PER_SEC) // 5 seconds delay

int main(void) {
    // Initialize DHT sensor
    dht_t dev;
    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        printf("Error: Unable to initialize DHT sensor\n");
        return 1;
    }
    
    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        int16_t temperature, humidity;

        // Read DHT sensor
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            printf("Error: Unable to read data from DHT sensor\n");
        } else {
            printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
            
            // Check temperature and control LED
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Warning: Temperature exceeds threshold! LED is ON\n");
            } else {
                gpio_clear(LED_PIN);
                printf("Temperature is within safe range. LED is OFF\n");
            }
        }

        // Wait for the next cycle
        xtimer_sleep(DELAY);
    }
    return 0;
}