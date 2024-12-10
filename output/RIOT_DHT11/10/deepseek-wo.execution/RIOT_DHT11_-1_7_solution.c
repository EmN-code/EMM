
#include "periph/gpio.h"
#include "xtimer.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static void _dht_read(int16_t *temp, int16_t *hum) {
    // Simulated DHT11 reading function
    // Replace this with actual DHT11 reading logic if available
    *temp = 250; // 25.0°C
    *hum = 600;  // 60.0%
}

int main(void) {
    int16_t temperature, humidity;

    // Initialize GPIO pin for DHT11
    gpio_init(DHT_PIN, GPIO_OUT);

    while (1) {
        // Read DHT11 sensor data
        _dht_read(&temperature, &humidity);

        // Print the temperature and humidity values
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
               temperature / 10, temperature % 10,
               humidity / 10, humidity % 10);

        // Wait for 5 seconds
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
