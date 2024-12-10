#include <xtimer.h>
#include <msg.h>
#include <dht.h>
#include <periph/gpio.h>

#define DHT_PIN             GPIO_PIN(PORT_B, 5)  // Correctly define the port and pin for DHT sensor
#define LED_PIN             GPIO_PIN(PORT_A, 0)  // Correctly define the port and pin for LED
#define TEMP_THRESHOLD      30 // set temperature threshold
#define INTERVAL            (5U * 1000000U) // 5 seconds in microseconds

int main(void) {
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    // Initialize the DHT sensor
    dht_t dev;
    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read from sensor\n");
            continue; // retry reading from the sensor
        }
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp/10, temp%10, hum/10, hum%10);

        // Check temperature and toggle LED
        if (temp / 10 > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
            printf("Temperature exceeded threshold! LED ON\n");
        } else {
            gpio_clear(LED_PIN);
            printf("Temperature below threshold. LED OFF\n");
        }

        // Sleep for 5 seconds
        xtimer_usleep(INTERVAL);
    }
    return 0;
}