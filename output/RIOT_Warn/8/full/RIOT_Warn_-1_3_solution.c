#include "string.h"
#include "dht_params.h"
#include "dht.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "periph/i2c.h"

#define DHT_PIN GPIO_PIN(0, 4) // Assumed GPIO pin for DHT sensor
#define LED_PIN GPIO_PIN(0, 5) // Assumed GPIO pin for LED
#define TEMP_THRESHOLD 30 // Temperature threshold in degree Celsius

void _xtimer_sleep(unsigned seconds) {
    vTaskDelay(pdMS_TO_TICKS(seconds * 1000));
}

int main(void) {
    // Initialize the GPIO for LED
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Optionally, configure the DHT sensor here if required
    dht_params_t dht_params;
    dht_init(&dht_params, DHT_PIN, DHT11);

    while (1) {
        int16_t temp, hum;

        // Read the temperature and humidity from the DHT11 sensor
        if (dht_read(&dht_params, &temp, &hum) != DHT_OK) {
            printf("Error reading DHT11 sensor. Trying again in 1 second.\n");
            _xtimer_sleep(1);
            continue;
        }

        // Convert temperature to Celsius
        temp /= 10; // Assuming the sensor gives temp*10

        // Check if temperature exceeds the threshold
        if (temp > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
            printf("Warning: Temperature (%dC) exceeds threshold! LED turned on.\n", temp);
        } else {
            gpio_clear(LED_PIN);
            printf("Temperature (%dC) is below the threshold. LED turned off.\n", temp);
        }

        // Sleep for 5 seconds before the next reading
        _xtimer_sleep(5);
    }
    return 0;
}