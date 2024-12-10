#include <dht.h>
#include <periph/gpio.h>
#include <xtimer.h>

#define DHT_PIN GPIO_PIN(GPIO_PORT_X, Y)  // Replace X and Y with actual port/pin numbers
#define LED_PIN GPIO_PIN(GPIO_PORT_A, B) // Replace A and B with actual port/pin numbers
#define TEMP_THRESHOLD 30  // Define the temperature threshold

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    // Initialize DHT sensor
    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    // Initialize LED GPIO
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                    temp / 10, temp % 10, hum / 10, hum % 10);
            
            // Check temperature threshold
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN); // Turn on LED
            } else {
                gpio_clear(LED_PIN); // Turn off LED
            }
        } else {
            printf("Failed to read DHT sensor\n");
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}