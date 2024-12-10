#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 9)  // Replace PORT_A, 9 with your actual port and pin
#define LED_PIN GPIO_PIN(PORT_B, 5)  // Replace PORT_B, 5 with your actual port and pin
#define TEMP_THRESHOLD 30            // Temperature threshold, adjust as needed (30°C here)
#define SLEEP_INTERVAL (5U * US_PER_SEC)  // 5 seconds sleep interval

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    dht_t dev;
    dht_params_t params = {
        .pin = DHT_PIN,
        .type = DHT11
    };
    
    if (dht_init(&dev, &params) != DHT_OK) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }

    int16_t temp, hum;
    while (1) {
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read from DHT11\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        xtimer_sleep(SLEEP_INTERVAL);
    }
    return 0;
}