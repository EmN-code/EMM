#include <xtimer.h>
#include <stdio.h>
#include <dht.h>
#include <periph/gpio.h>

#define DHT11_PIN GPIO_PIN(0, 2)

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;
    
    puts("DHT11 test application
");

    if (dht_init(&dev, DHT11_PIN) != DHT_OK) {
        puts("Failed to initialize DHT11 sensor");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            puts("Failed to read data from DHT11 sensor.");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%
",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        }

        xtimer_sleep(5); // Read the sensor every 5 seconds
    }

    return 0;
}
