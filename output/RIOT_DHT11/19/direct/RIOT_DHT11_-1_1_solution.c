#include <xtimer.h>
#include <stdio.h>
#include <dht.h>
#include <periph/gpio.h>

#define DHT11_GPIO_PIN GPIO_PIN(0, 2)  // Define the GPIO pin where the DHT11 is connected
#define INTERVAL (5U * US_PER_SEC)    // 5 seconds interval

int main(void) {
    dht_params_t params;
    params.pin = DHT11_GPIO_PIN;
    params.type = DHT11;
    
    if (dht_init(&params) != DHT_OK) {
        puts("Error: Could not initialize DHT11 sensor");
        return 1;
    }

    while (1) {
        int16_t temperature, humidity;

        if (dht_read(&params, &humidity, &temperature) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        } else {
            puts("Error: Could not read from DHT11 sensor");
        }

        xtimer_sleep(INTERVAL);
    }

    return 0;
}