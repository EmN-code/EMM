#include "xtimer.h"
#include "thread.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 2)  // Assume the DHT is connected to GPIO_A2
#define LED_PIN GPIO_PIN(PORT_A, 3)  // Assume the LED is connected to GPIO_A3
#define TEMP_THRESHOLD 30  // Temperature threshold for LED warning
#define INTERVAL (5U * US_PER_SEC)  // 5 seconds interval in microseconds

int main(void) {
    dht_t dev;
    int16_t temp, hum;
    dht_params_t params = {
        .pin = DHT_PIN,
        .type = DHT_TYPE_DHT11
    };

    // Initialize the DHT sensor
    if (dht_init(&dev, &params) != DHT_OK) {
        puts("Error initializing DHT sensor\n");
        return 1;
    }

    // Initialize the LED pin
    if (gpio_init(LED_PIN, GPIO_OUT) != 0) {
        puts("Error initializing LED pin\n");
        return 1;
    }

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            puts("Error reading from DHT sensor\n");
        } else {
            printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum);
            // Check the temperature and turn on/off LED as needed
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                puts("LED ON: Temperature above threshold\n");
            } else {
                gpio_clear(LED_PIN);
                puts("LED OFF: Temperature below threshold\n");
            }
        }

        // Wait for the next interval
        xtimer_sleep(INTERVAL);
    }

    return 0;
}