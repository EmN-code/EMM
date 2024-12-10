#include <xtimer.h>
#include <dht.h>
#include <periph/gpio.h>

#define DHT_PIN GPIO_PIN(0, 2) // GPIO pin 2
#define INTERVAL 5

static dht_t dev;

void read_dht(void)
{
    int16_t temperature, humidity;
    if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
        printf("Temperature: %d.%d°C\n", temperature / 10, abs(temperature % 10));
        printf("Humidity: %d.%d%%\n", humidity / 10, abs(humidity % 10));
    }
    else {
        printf("Could not read data from sensor\n");
    }
}

int main(void)
{
    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    // Periodically read the sensor every five seconds
    while (1) {
        read_dht();
        xtimer_sleep(INTERVAL);
    }

    return 0;
}