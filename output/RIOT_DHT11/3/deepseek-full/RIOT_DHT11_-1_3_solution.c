
#include "periph/gpio.h"
#include "xtimer.h"
#include <stdio.h>

#define DHT11_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

int main(void)
{
    gpio_init(DHT11_PIN, GPIO_OUT);
    gpio_set(DHT11_PIN);

    while (1) {
        // Simulate reading DHT11
        int temperature = 25; // Simulated temperature value
        int humidity = 60;    // Simulated humidity value

        printf("Temperature: %d C, Humidity: %d%%\n", temperature, humidity);

        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}
