#include <xtimer.h>  // For xtimer_usec, xtimer_sleep
#include <stdio.h>    // For printf
#include <dht.h>      // For dht_t, dht_init, dht_read
#include <periph/gpio.h>  // For gpio_t

#define DHT11_PIN GPIO_PIN(0, 2)  // GPIO2 on ESP32
#define READ_INTERVAL 5  // Read every 5 seconds

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    // Initialize the DHT11 sensor
    if (dht_init(&dev, DHT11_PIN) != 0) {
        printf("Error: Could not initialize DHT11\n");
        return -1;
    }
    
    while (1) {
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Error: Could not read data from DHT11\n");
        }
        xtimer_sleep(READ_INTERVAL);
    }

    return 0;
}