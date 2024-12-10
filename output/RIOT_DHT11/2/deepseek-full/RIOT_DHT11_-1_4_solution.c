
#include <stdio.h>
#include <unistd.h>
#include "periph/gpio.h"
#include "xtimer.h"

#define DHT11_PIN GPIO_PIN(0, 2)
#define INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

void read_dht11(void) {
    // Dummy function to simulate DHT11 reading
    printf("DHT11 read: Temperature = 25°C, Humidity = 50%%\n");
}

int main(void) {
    // Initialize GPIO pin for DHT11
    if (gpio_init(DHT11_PIN, GPIO_OUT) < 0) {
        printf("Failed to initialize GPIO\n");
        return 1;
    }

    while (1) {
        read_dht11();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
