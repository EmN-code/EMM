#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"
#include "stdio.h"

#define DHT_PIN GPIO_PIN(0, 4) // D4 pin on ESP32
#define LED_PIN GPIO_PIN(0, 5) // D5 pin on ESP32
#define THRESHOLD 30.0 // Temperature threshold
#define INTERVAL (5U * US_PER_SEC) // 5 seconds interval

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    dht_t dev;
    int16_t temperature, humidity;
    int rc;
    
    while(1) {
        if ((rc = dht_init(&dev, DHT11, DHT_PIN)) != DHT_OK) {
            printf("Failed to initialize DHT sensor\n");
            xtimer_sleep(1);
            continue;
        }
        
        // Read the sensor values
        if ((rc = dht_read(&dev, &temperature, &humidity)) != DHT_OK) {
            printf("Failed to read DHT sensor\n");
            xtimer_sleep(1);
            continue;
        }

        printf("Temperature: %.1f°C, Humidity: %.1f%%\n", temperature / 10.0, humidity / 10.0);

        // Check temperature and control LED
        if (temperature / 10.0 > THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }

        // Wait for the next measurement
        xtimer_sleep(5);
    }
    return 0;
}