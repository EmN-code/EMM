#include "periph_conf.h"
#include "xtimer.h"
#include "timex.h"
#include "stdio.h"
#include "periph/gpio.h"

#define DHT11_PIN GPIO_PIN(PORT_A, 0)  // Assuming PORT_A and pin 0 for DHT11
#define LED_PIN GPIO_PIN(PORT_B, 1)    // Assuming PORT_B and pin 1 for LED

#define TEMP_THRESHOLD 30              // Temperature threshold for the LED warning
#define READ_INTERVAL (5U * US_PER_SEC) // Read every 5 seconds

void initialize_dht_and_read(void) {
    int16_t temperature, humidity;

    // Initialize DHT11 sensor (correct without additional argument as per example)
    if (dht_init(&dht_dev, DHT11_PIN) != 0) {
        printf("Failed to initialize DHT11 sensor\n");
        return;
    }

    while (1) {
        // Read data from DHT11 sensor
        if (dht_read(&dht_dev, &humidity, &temperature) != DHT_SUCCESS) {
            printf("DHT11 read error\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                    temperature / 10, temperature % 10,
                    humidity / 10, humidity % 10);

            // Check if temperature exceeds the threshold
            if ((temperature / 10) > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        
        xtimer_sleep(READ_INTERVAL);
    }
}

int main(void) {
    // Initialize GPIO for LED
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error to initialize LED pin\n");
        return 1;
    }

    initialize_dht_and_read();

    return 0;
}