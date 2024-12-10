#include <xtimer.h>
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 0)   // Use appropriate port
#define LED_PIN GPIO_PIN(PORT_B, 1)   // Adjust PORT to match your board configuration
#define TEMP_THRESHOLD 30             // Celsius

static dht_t dht_dev;

void handle_temperature(void) {
    int16_t temp, hum;
    int rc;

    while (1) {
        // Initialize the DHT sensor
        rc = dht_init(&dht_dev, DHT_TYPE_DHT11, DHT_PIN);
        if (rc != 0) {
            printf("DHT initialization failed\n");
            xtimer_sleep(1);
            continue;
        }

        // Read temperature and humidity
        rc = dht_read(&dht_dev, &temp, &hum);
        if (rc != 0) {
            printf("DHT read failed\n");
            xtimer_sleep(1);
            continue;
        }

        printf("Temp: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        
        // Turn on the LED if temperature exceeds the threshold
        if ((temp / 10) > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }

        xtimer_sleep(5); // Wait for 5 seconds
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    handle_temperature();
    return 0;
}