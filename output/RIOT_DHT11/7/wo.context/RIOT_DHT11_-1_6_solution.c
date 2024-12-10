#include "xtimer.h"
#include "periph/gpio.h"
#include <stdio.h>

#define DHT11_PIN GPIO_PIN(0, 2) // GPIO2
#define READ_INTERVAL (5U * 1000000U) // 5 seconds in microseconds

static void read_dht11(void)
{
    // Simulate reading from DHT11 sensor
    int humidity = 60; // mock value
    int temperature = 25; // mock value

    printf("DHT11 Sensor Read - Temperature: %d C, Humidity: %d%%\n", temperature, humidity);
}

int main(void)
{
    printf("Starting DHT11 sensor read every 5 seconds on GPIO2\n");

    // Initialize GPIO pin for DHT11
    if (gpio_init(DHT11_PIN, GPIO_IN) < 0) {
        printf("Error: Unable to initialize GPIO\n");
        return -1;
    }

    // Continuous loop
    while (1) {
        read_dht11();
        xtimer_usleep(READ_INTERVAL);
    }

    return 0;
}
