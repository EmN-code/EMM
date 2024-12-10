
#include "periph/gpio.h"
#include "ztimer.h"
#include "timex.h"

#define DHT11_PIN GPIO_PIN(0, 2)

void read_dht11(void) {
    // Simulate reading DHT11 data
    printf("Reading DHT11 data...\n");
    // Placeholder for actual DHT11 reading logic
}

int main(void) {
    // Initialize GPIO pin for DHT11
    if (gpio_init(DHT11_PIN, GPIO_OUT) < 0) {
        printf("Failed to initialize GPIO\n");
        return 1;
    }

    while (1) {
        read_dht11();
        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5 * MS_PER_SEC);
    }

    return 0;
}
