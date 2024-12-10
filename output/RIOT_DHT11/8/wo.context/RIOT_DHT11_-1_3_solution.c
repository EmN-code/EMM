#include <xtimer.h>  // Timer related functions
#include <stdio.h>   // Standard input/output functions
#include <dht.h>     // DHT sensor functions
#include <periph/gpio.h> // GPIO functions

#define DHT11_PIN GPIO_PIN(0, 2) // Pin 2 on port 0 (ESP32 GPIO 2)
#define INTERVAL (5U * US_PER_SEC) // Interval of 5 seconds

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    // Initialize the DHT11 sensor
    if (dht_init(&dev, DHT11_PIN) != DHT_OK) {
        printf("Error: Unable to initialize DHT11 sensor\n");
        return 1;
    }

    printf("DHT11 sensor initialization successful. Reading values every 5 seconds.\n");

    // Main loop
    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Error: Unable to read from DHT11 sensor\n");
        }

        // Sleep for the defined interval
        xtimer_sleep(INTERVAL / US_PER_SEC); 
    }

    return 0;
}