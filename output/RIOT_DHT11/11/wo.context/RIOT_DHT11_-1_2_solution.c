#include "xtimer.h"
#include "thread.h"
#include "periph_conf.h"
#include "periph/gpio.h"

#include <stdio.h>

#define DHT11_PIN GPIO_PIN(0, 2) // GPIO number 2
#define DHT11_READ_INTERVAL (5 * US_PER_SEC) // 5 seconds in microseconds

static void read_dht11(void) {
    // Simulate DHT11 read function
    printf("Reading DHT11 at GPIO %d...\n", DHT11_PIN);
    // Placeholder for actual DHT11 reading logic
}

int main(void) {
    // Initialize DHT11 GPIO pin
    gpio_init(DHT11_PIN, GPIO_IN);

    while (1) {
        read_dht11();
        xtimer_usleep(DHT11_READ_INTERVAL);
    }

    return 0;
}