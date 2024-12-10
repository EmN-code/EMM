
#include <stdio.h>
#include <periph/gpio.h>
#include <ztimer.h>

#define DHT11_PIN GPIO_PIN(0, 2)
#define INTERVAL (5 * 1000U) // 5 seconds in milliseconds

static void read_dht11(void) {
    // Simulate reading DHT11 data
    printf("Simulated DHT11 reading\n");
}

int main(void) {
    // Initialize the GPIO pin for DHT11
    if (gpio_init(DHT11_PIN, GPIO_OUT) < 0) {
        printf("Failed to initialize GPIO\n");
        return 1;
    }

    while (1) {
        // Read DHT11 data
        read_dht11();

        // Wait for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);
    }

    return 0;
}
