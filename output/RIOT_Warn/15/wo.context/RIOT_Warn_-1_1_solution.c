// Include headers in descending alphabetical order by their first letter.
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"
#include "dht_params.h"

// Define the GPIO pin for the LED
#define LED_PIN         GPIO_PIN(PORT, PIN)

// Define the threshold temperature
#define TEMP_THRESHOLD  30

// Define the sleep interval in microseconds (5 seconds)
#define SLEEP_INTERVAL  (5U * US_PER_SEC)

// Helper function to handle potential failure in DHT reading repeatedly
static inline int safe_dht_read(dht_t *dev, int16_t *temp, int16_t *hum) {
    int res;
    while (true) {
        res = dht_read(dev, temp, hum);
        if (res == 0) {
            break;
        }
        printf("DHT read failed, retrying...\n");
        xtimer_sleep(1);
    }
    return res;
}

int main(void) {
    // Initialize the LED pin as output
    if (gpio_init(LED_PIN, GPIO_OUT) < 0) {
        printf("Failed to initialize LED pin\n");
        return 1;
    }

    // Set LED to low as default
    gpio_clear(LED_PIN);

    // Initialize DHT sensor
    dht_t sensor;
    if (dht_init(&sensor, &dht_params[0]) < 0) {
        printf("DHT init failed\n");
        return 1;
    }

    while (1) {
        int16_t temp, hum;
        safe_dht_read(&sensor, &temp, &hum);
        
        // Check the temperature threshold
        if (temp > (TEMP_THRESHOLD * 10)) {  // DHT returns temperature in tenths of degree
            gpio_set(LED_PIN);
            printf("Temperature: %d.%d°C, warning LED ON\n", temp / 10, temp % 10);
        } else {
            gpio_clear(LED_PIN);
            printf("Temperature: %d.%d°C, LED OFF\n", temp / 10, temp % 10);
        }

        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}