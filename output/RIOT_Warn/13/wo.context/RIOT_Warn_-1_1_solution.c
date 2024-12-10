#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN         GPIO_PIN(PORT_B, 5) // Define DHT pin
#define LED_PIN         GPIO_PIN(PORT_B, 10) // Define LED pin
#define TEMP_THRESHOLD  30 // Temperature threshold
#define READ_INTERVAL   (5U * US_PER_SEC) // 5 seconds interval

int main(void)
{
    dht_t dev;
    int16_t temperature, humidity;

    // Initialize DHT sensor
    if (dht_init(&dev, &dht_params[0]) != DHT_OK) {
        puts("Failed to initialize DHT sensor");
        return 1;
    }

    // Initialize LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    while (1) {
        // Read temperature and humidity values
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            puts("DHT read failed");
        } else {
            printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);
            
            // Check temperature and activate LED if necessary
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        // Wait for the next interval
        xtimer_sleep(READ_INTERVAL / US_PER_SEC);
    }

    return 0;
}