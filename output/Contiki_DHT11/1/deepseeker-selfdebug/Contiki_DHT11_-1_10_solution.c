
#include <stdio.h>
#include <xtimer.h>
#include <periph/gpio.h>

#define DHT11_GPIO_PIN GPIO_PIN(0, 2)
#define DHT11_READ_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

typedef struct {
    int16_t temperature;
    int16_t humidity;
} dht11_data_t;

int dht11_read(gpio_t pin, dht11_data_t *data) {
    // Simulated DHT11 read function
    // Replace this with actual DHT11 reading logic
    data->temperature = 250; // 25.0°C
    data->humidity = 600;     // 60.0%
    return 0;
}

int main(void) {
    dht11_data_t dht11_data;

    // Initialize GPIO pin for DHT11
    if (gpio_init(DHT11_GPIO_PIN, GPIO_OUT) < 0) {
        puts("Error initializing GPIO pin for DHT11");
        return 1;
    }

    while (1) {
        if (dht11_read(DHT11_GPIO_PIN, &dht11_data) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   dht11_data.temperature / 10, dht11_data.temperature % 10,
                   dht11_data.humidity / 10, dht11_data.humidity % 10);
        } else {
            puts("Failed to read DHT11 sensor");
        }

        xtimer_usleep(DHT11_READ_INTERVAL);
    }

    return 0;
}
