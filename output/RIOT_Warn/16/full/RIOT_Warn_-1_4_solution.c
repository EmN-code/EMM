#include <xtimer.h>  // for sleeping and timer management
#include <stdio.h>   // for input/output functions
#include <periph/gpio.h>  // for gpio handling functions
#include <dht.h>      // for dht sensor functions

#define DHT_PIN GPIO_PIN(PORT_X, 0) // Assuming DHT11 sensor is connected to port X, pin 0
#define LED_PIN GPIO_PIN(PORT_Y, 0) // Assuming the LED is connected to port Y, pin 0
#define TEMP_THRESHOLD 30           // Threshold temperature for turning on the LED
#define SLEEP_INTERVAL (5U * US_PER_SEC) // 5 seconds interval

int main(void)
{
    gpio_init(LED_PIN, GPIO_OUT);
    dht_t dev;
    int16_t temp, hum;
    int rc;

    // Initialize the sensor
    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        puts("DHT sensor initialization failed");
        return 1;
    }
    
    while (1) {
        // Read data in a retry loop due to possible failures
        rc = DHT_ERR_TIMEOUT; // Initialize to error state
        while (rc == DHT_ERR_TIMEOUT) {
            rc = dht_read(&dev, &temp, &hum);
            if (rc == DHT_ERR_TIMEOUT) {
                printf("Reading sensor data failed. Retrying...\n");
                xtimer_sleep(1); // Sleep for a second before retrying
            }
            else if (rc != DHT_OK) {
                printf("An error occurred while reading the sensor. Error code: %d\n", rc);
                break; // In case of a different error, break out of the loop
            }
        }

        if (rc == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);

            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        xtimer_sleep(SLEEP_INTERVAL / 1000000); // Sleep for an interval of 5 seconds
    }

    return 0;
}