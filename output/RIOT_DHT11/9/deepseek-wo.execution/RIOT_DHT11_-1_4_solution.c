
#include <periph/gpio.h>
#include <xtimer.h>
#include <stdio.h>

#define DHT11_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

void read_dht11(void) {
    // Simulate reading DHT11 data
    printf("Reading DHT11 data...\n");
    // In a real application, you would read the actual data from the DHT11 sensor
    printf("Temperature: 25°C, Humidity: 50%%\n");
}

int main(void) {
    // Initialize the GPIO pin for the DHT11 sensor
    if (gpio_init(DHT11_PIN, GPIO_OUT) < 0) {
        printf("Failed to initialize GPIO pin\n");
        return 1;
    }

    while (1) {
        read_dht11();
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}
