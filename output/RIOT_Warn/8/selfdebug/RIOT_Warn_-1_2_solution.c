#include <xtimer.h>
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 4)  // Define the DHT11 data pin
#define LED_PIN GPIO_PIN(0, 2)  // Define the LED pin
#define TEMP_THRESHOLD 30       // Temperature threshold in Celsius

void check_temperature(void) {
    int16_t temperature, humidity;
    dht_t dev;
    dht_params_t params;
    params.pin = DHT_PIN;

    dht_init(&dev, &params);

    if (dht_read(&dev, &temperature, &humidity) == 0) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
               temperature / 10, temperature % 10, 
               humidity / 10, humidity % 10);

        if (temperature / 10 > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
    } else {
        printf("Failed to read data from DHT11
");
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    
    while (1) {
        check_temperature();
        xtimer_sleep(5);  // Sleep for 5 seconds
    }
    return 0;
}