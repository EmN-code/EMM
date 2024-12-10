
#include "periph/gpio.h"
#include "ztimer.h"
#include "timex.h"
#include "stdio.h"

#define DHT11_PIN GPIO_PIN(0, 2)
#define INTERVAL (5 * US_PER_SEC)

int main(void) {
    if (gpio_init(DHT11_PIN, GPIO_OUT) < 0) {
        puts("Failed to initialize GPIO");
        return 1;
    }

    while (1) {
        // Simulate reading DHT11 data
        puts("Reading DHT11 data...");

        // Simulate a delay for reading the sensor
        ztimer_sleep(ZTIMER_USEC, INTERVAL);
    }

    return 0;
}
