
#include "periph/gpio.h"
#include "xtimer.h"

#define DHT_PIN             GPIO_PIN(0, 2)
#define SLEEP_INTERVAL      (5 * 1000000U)  // 5 seconds in microseconds

int main(void)
{
    int16_t temperature, humidity;

    // Initialize the GPIO pin for the DHT11 sensor
    gpio_init(DHT_PIN, GPIO_OUT);

    while (1) {
        // Read the DHT11 sensor
        if (dht_read(DHT_PIN, &temperature, &humidity) == 0) {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
        } else {
            puts("Failed to read from DHT11 sensor");
        }

        // Sleep for 5 seconds
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}
