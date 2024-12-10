#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(PORT_A, 6)  // Correctly declaring the GPIO_PIN
#define LED_PIN GPIO_PIN(PORT_A, 7)
#define TEMPERATURE_THRESHOLD 30

int main(void) {
    dht_t dev;
    dht_params_t dht_params;
    dht_params.pin = DHT_PIN;
    dht_params.type = DHT11;
    int16_t temperature, humidity;

    /* Initialize GPIO for LED */
    gpio_init(LED_PIN, GPIO_OUT);

    /* Initialize DHT11 sensor */
    if (dht_init(&dev, &dht_params) != DHT_OK) {
        puts("DHT initialization failed");
        return 1;
    }
    puts("DHT initialized");

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            puts("DHT read failed");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
            if (temperature / 10 > TEMPERATURE_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        xtimer_sleep(5);
    }
    return 0;
}