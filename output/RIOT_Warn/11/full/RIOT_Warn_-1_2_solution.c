#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_params.h"
#include "dht_params.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 0)
#define LED_PIN GPIO_PIN(PORT_B, 1)
#define TEMP_THRESHOLD 30 // setting the temperature threshold to 30°C

void setup_gpio(void) {
    // Initialize the GPIO for the DHT sensor
    if (gpio_init(DHT_PIN, GPIO_IN) < 0) {
        printf("Error initializing GPIO for DHT sensor\n");
    }
    // Initialize the GPIO for the LED
    if (gpio_init(LED_PIN, GPIO_OUT) < 0) {
        printf("Error initializing GPIO for LED\n");
    }
}

void check_dht_sensor(void) {
    dht_t dev;
    int16_t temp, hum;
    int res;

    // Initialize the DHT sensor
    if (dht_init(&dev, &dht_params[0]) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return;
    }

    while (1) {
        // Read data from the DHT sensor
        if ((res = dht_read(&dev, &temp, &hum)) != DHT_OK) {
            printf("DHT read failed\n");
        }
        else {
            // Convert temperature to Celsius and check against threshold
            temp = temp / 10; // DHT returns temperature multiplied by 10
            printf("Temperature: %d C, Humidity: %d %%\n", temp, hum);

            // Turn on LED if temperature exceeds threshold
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        // Sleep for 5 seconds
        xtimer_sleep(5);
    }
}

int main(void) {
    setup_gpio();
    check_dht_sensor();
    return 0;
}