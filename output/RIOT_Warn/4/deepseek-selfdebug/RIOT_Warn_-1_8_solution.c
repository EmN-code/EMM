
#include <stdio.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN             GPIO_PIN(0, 12)  // GPIO12 on ESP32
#define LED_PIN             GPIO_PIN(0, 2)   // GPIO2 on ESP32
#define TEMP_THRESHOLD      30               // Temperature threshold in Celsius
#define INTERVAL_MS         5000000          // 5 seconds interval

int main(void)
{
    dht_t dev;
    int16_t temp, hum;
    bool led_state = false;

    // Initialize DHT11 sensor
    if (dht_init(&dev, DHT_PIN) == DHT_OK) {
        printf("DHT11 sensor initialized successfully\n");
    } else {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    // Initialize LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);

            // Check if temperature exceeds the threshold
            if (temp / 10 > TEMP_THRESHOLD) {
                if (!led_state) {
                    gpio_set(LED_PIN);  // Turn on LED
                    led_state = true;
                }
            } else {
                if (led_state) {
                    gpio_clear(LED_PIN);  // Turn off LED
                    led_state = false;
                }
            }
        } else {
            printf("Failed to read data from DHT11 sensor\n");
        }

        // Wait for 5 seconds
        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
