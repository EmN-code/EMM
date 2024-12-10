#include <xtimer.h>
#include <periph/gpio.h>
#include <dht_params.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(0, 2)  // GPIO 2
#define READ_INTERVAL (5U * US_PER_SEC)  // Interval to read the sensor

static dht_t dev;

int main(void)
{
    puts("ESP32 DHT11 Reader");

    // Initialize DHT11 sensor
    dht_params_t params = {
        .pin = DHT_PIN,
        .type = DHT11
    };

    if (dht_init(&dev, &params) != DHT_OK) {
        puts("Error initializing DHT11");
        return 1;
    }

    // Read loop
    while (1) {
        int16_t temperature, humidity;

        // Read sensor data
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d°C, Humidity: %d%%\n", temperature / 10, humidity / 10);
        } else {
            puts("Failed to read DHT11 sensor");
        }

        // Wait for the next measurement
        xtimer_sleep(5);
    }

    return 0;
}