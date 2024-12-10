#include <xtimer.h>  // for using xtimer_sleep function
#include <stdio.h>   // for printf function
#include <periph/gpio.h>  // for using GPIO functions and constants
#include <dht.h>     // for using DHT functionalities

#define DHT_PIN GPIO_PIN(0, 17)  // Define the DHT11 data pin (adjust the pin as per your setup)
#define LED_PIN GPIO_PIN(0, 18)  // Define the LED pin (adjust the pin as per your setup)
#define TEMP_THRESHOLD 25  // Define the temperature threshold

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    dht_t dev;
    dht_params_t params = {
        .pin = DHT_PIN,
        .type = DHT11
    };
    dht_init(&dev, &params);  // Corrected usage of dht_init

    while (1) {
        int16_t temp, hum;

        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, 
                   hum / 10, hum % 10);

            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Error reading from DHT11 sensor\n");
        }

        xtimer_sleep(5);  // Delay before reading again
    }

    return 0;
}