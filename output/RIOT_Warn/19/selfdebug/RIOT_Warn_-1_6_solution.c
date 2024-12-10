#include "xtimer.h"
#include "periph/gpio.h" // for controlling the LED
#include "dht.h" // for interfacing with the DHT11 sensor

#define DHT11_PIN GPIO_PIN(0, 5) // Define the pin where the DHT11 is connected
#define LED_PIN GPIO_PIN(0, 6) // Define the pin where the LED is connected
#define TEMP_THRESHOLD 25 // Set the temperature threshold

void read_temperature(void) {
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) == DHT_OK) {
        // Check the temperature and turn on LED if it exceeds threshold
        if (temp > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);

    dht_t dev;
    if (dht_init(&dev, DHT11_PIN, DHT11) != 0) {
        puts("Error initializing DHT11");
    }

    while (1) {
        read_temperature();
        xtimer_sleep(5);
    }
    return 0;
}