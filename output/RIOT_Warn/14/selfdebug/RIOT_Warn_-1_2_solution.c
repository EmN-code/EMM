#include "xtimer.h"
#include "dht.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(0, 4) // Specify the exact GPIO pin attached to the DHT22 sensor
#define LED_PIN GPIO_PIN(0, 5) // Specify the GPIO pin connected to the LED
#define TEMP_THRESHOLD 300     // Temperature threshold in deci-degrees Celsius (30.0 degrees Celsius)

int main(void) {
    int16_t temperature, humidity;
    dht_t dev;
    int res;

    // Initialize the GPIO for LED as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize DHT sensor
    if (dht_init(&dev, DHT_PIN) != 0) {
        printf("Error initializing DHT sensor\n");
        return 1;
    }

    while (1) {
        res = dht_read(&dev, &temperature, &humidity);
        if (res == 0) {
            printf("Temperature: %d.%d°C\n", temperature / 10, temperature % 10);
            printf("Humidity: %d.%d%%\n", humidity / 10, humidity % 10);
            // Check temperature threshold
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Warning: Temperature exceeded threshold. LED is ON.\n");
            } else {
                gpio_clear(LED_PIN);
                printf("Temperature is below threshold. LED is OFF.\n");
            }
        } else {
            printf("Error reading from DHT sensor\n");
        }
        // Sleep for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}