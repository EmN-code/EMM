#include <stdio.h>
#include <xtimer.h>
#include <dht.h>
#include <periph/gpio.h>

#define DHT_PIN GPIO_PIN(0, 2) // Assuming the DHT11 is connected to GPIO2

int main(void){
    dht_t dev;
    int16_t temperature, humidity;
    int result;

    // Initialize DHT
    if (dht_init(&dev, DHT_PIN) != 0) {
        puts("DHT initialization failed");
        return 1;
    }

    puts("Reading DHT11 sensor every 5 seconds...");
    while (1) {
        result = dht_read(&dev, &temperature, &humidity);
        if (result == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        } else {
            printf("DHT reading failed with code %d\n", result);
        }
        xtimer_sleep(5);
    }

    return 0;
}

// Note: Always ensure your `dht_read` call matches your specific DHT library and definition.