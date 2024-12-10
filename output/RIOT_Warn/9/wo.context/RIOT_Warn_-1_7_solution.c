#include "xtimer.h"
#include "timex.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT11_PIN GPIO_PIN(0, 4) // Adjust pin as per your connection
#define LED_PIN GPIO_PIN(0, 5) // Adjust pin as per your connection
#define TEMP_THRESHOLD 30 // Temperature threshold in Celsius
#define INTERVAL (5U * US_PER_SEC) // 5 seconds interval

int main(void) {
    // Initialize the DHT sensor and GPIO
    dht_t dev;
    int16_t temp, hum;
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    // Main loop
    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            // Check temperature and turn on LED if it exceeds the threshold
            if (temp > TEMP_THRESHOLD * 10) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }
    return 0;
}