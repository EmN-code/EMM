#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT, 0)  // Update according to the actual port number
#define LED_PIN GPIO_PIN(PORT, 1)  // Update according to the actual port number
#define TEMP_THRESHOLD 30          // Temperature threshold for LED warning
#define INTERVAL 5 * 1000000       // 5 seconds interval in microseconds

int main(void) {
    dht_t dev;
    gpio_init(LED_PIN, GPIO_OUT);
    int16_t temp, hum;
    
    if (dht_init(&dev, DHT11, DHT_PIN) != DHT_OK) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp/10, temp%10, hum/10, hum%10);
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read data from DHT11\n");
        }

        xtimer_sleep(xtimer_usec_from_usec(INTERVAL));
    }
    
    return 0;
}