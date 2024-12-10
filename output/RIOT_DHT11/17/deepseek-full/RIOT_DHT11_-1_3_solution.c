
#include <stdio.h>
#include "periph/gpio.h"
#include "ztimer.h"

#define DHT11_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

void read_dht11(void) {
    int16_t temp, hum;
    gpio_init(DHT11_PIN, GPIO_OUT);
    gpio_set(DHT11_PIN);
    ztimer_sleep(ZTIMER_USEC, 2000); // 2 ms delay
    gpio_init(DHT11_PIN, GPIO_IN);

    // DHT11 reading logic (simplified for example)
    // This part should be replaced with actual DHT11 reading code
    temp = 25; // Dummy temperature value
    hum = 60;  // Dummy humidity value

    printf("Temperature: %d C, Humidity: %d %%\n", temp, hum);
}

int main(void) {
    puts("Starting DHT11 reading every 5 seconds");

    while (1) {
        read_dht11();
        ztimer_sleep(ZTIMER_USEC, SLEEP_INTERVAL);
    }

    return 0;
}
