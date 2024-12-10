#include "thread.h"
#include "dht.h"
#include "xtimer.h"

#define GPIO_PIN 2       // GPIO pin where DHT11 is connected
#define DHT_TYPE DHT11   // DHT11 sensor type
#define DELAY (5U * 1000000U) // 5 seconds delay in microseconds

int main(void)
{
    dht_t dev;
    dht_params_t params = {
        .pin = GPIO_PIN,
        .type = DHT_TYPE
    };

    // Initialize the DHT11 sensor
    if (dht_init(&dev, &params) != DHT_OK) {
        puts("Failed to initialize DHT11 sensor");
        return 1;
    }
    
    int16_t temperature, humidity;

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            puts("Failed to read data from DHT11 sensor");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        }

        // Wait for 5 seconds
        xtimer_usleep(DELAY);
    }

    return 0;
}