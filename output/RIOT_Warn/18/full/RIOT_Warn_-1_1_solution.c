#include <xtimer.h>
#include <stdio.h>
#include "dht.h"
#include "gpio.h"

#define DHT_PIN GPIO_PIN(0, 26)  // Adjust as needed
#define LED_PIN GPIO_PIN(0, 2)   // Adjust as needed
#define THRESHOLD_TEMP 30        // Temperature threshold
#define SLEEP_INTERVAL 5         // Sleep interval in seconds

int main(void) {
    printf("Starting DHT11 Monitoring...\n");
    
    dht_t dev;
    int res = dht_init(&dev, &(dht_params_t){.pin = DHT_PIN, .type = DHT11});
    if (res != 0) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }

    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                   temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp / 10 > THRESHOLD_TEMP) {
                gpio_set(LED_PIN);  // Turn LED on
            } else {
                gpio_clear(LED_PIN); // Turn LED off
            }
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}