
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <periph/gpio.h>
#include <xtimer.h>

#define DHT11_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

void read_dht11(void) {
    // Placeholder for DHT11 reading logic
    printf("Reading DHT11 sensor...\n");
    // Simulate reading process
    xtimer_usleep(100000); // 100ms delay to simulate reading
    printf("DHT11 read complete.\n");
}

int main(void) {
    // Initialize GPIO pin for DHT11
    if (gpio_init(DHT11_PIN, GPIO_OUT) < 0) {
        printf("Failed to initialize GPIO pin for DHT11\n");
        return 1;
    }

    while (1) {
        read_dht11();
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}
