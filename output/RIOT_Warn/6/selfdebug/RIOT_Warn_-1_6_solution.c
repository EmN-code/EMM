#include <xtimer.h>
#include <stdio.h>
#include <dht.h>
#include <periph/gpio.h>

#define DHT_PIN            GPIO_PIN(PORT_A, 0) // Define the correct port and pin for your setup
#define LED_PIN            GPIO_PIN(PORT_A, 1) // Define the correct port and pin for your LED
#define TEMP_THRESHOLD     30                   // Set the temperature threshold in Celsius

static dht_t dev;

void init_sensors(void) {
    if (dht_init(&dev, DHT_TYPE_DHT11) != DHT_OK) { // Correct the dht_init function usage
        printf("Error initializing DHT sensor\n");
        return;
    }
}

int main(void) {
    int16_t temp, hum;
    xtimer_ticks32_t last_wakeup = xtimer_now();

    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);  // Ensure LED is off initially

    init_sensors();

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temp: %d.%d°C, Hum: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn on LED if temperature exceeds threshold
            } else {
                gpio_clear(LED_PIN); // Turn off LED otherwise
            }
        } else {
            printf("Error reading DHT sensor\n");
        }

        // Wait for 5 seconds
        xtimer_periodic_wakeup(&last_wakeup, 5 * US_PER_SEC);
    }
    return 0;
}