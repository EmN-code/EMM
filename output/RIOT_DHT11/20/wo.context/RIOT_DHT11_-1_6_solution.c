# Corrected RIOT Firmware for Reading DHT11 on ESP32

#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)  // GPIO 2
#define DHT_TYPE DHT11          // Specify the sensor type
#define INTERVAL (5U * US_PER_SEC)

static dht_t dev;

int main(void) {
    int res;
    uint16_t humidity = 0;
    uint16_t temperature = 0;

    // Initialize the DHT sensor
    if ((res = dht_init(&dev, DHT_PIN, DHT_TYPE)) != DHT_OK) {
        printf("Error during DHT initialization: %d\n", res);
        return 1;
    }

    while (1) {
        // Read values from DHT sensor
        if ((res = dht_read(&dev, &humidity, &temperature)) != DHT_OK) {
            printf("Error during DHT read: %d\n", res);
        } else {
            printf("DHT values - Temperature: %u.%u°C, Humidity: %u.%u%%\n",
                   temperature / 10, temperature % 10, humidity / 10, humidity % 10);
        }
        
        // Sleep for INTERVAL
        xtimer_sleep(INTERVAL);
    }
    return 0;
}
