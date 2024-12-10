
#include "periph/gpio.h"
#include "xtimer.h"

#define DHT11_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static void read_dht11(void) {
    int16_t temperature, humidity;
    if (dht_read(DHT11, DHT11_PIN, &temperature, &humidity) == 0) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
               temperature / 10, temperature % 10,
               humidity / 10, humidity % 10);
    } else {
        puts("Failed to read from DHT11 sensor");
    }
}

int main(void) {
    // Initialize the DHT11 sensor
    if (gpio_init(DHT11_PIN, GPIO_OUT) < 0) {
        puts("Failed to initialize GPIO for DHT11");
        return 1;
    }

    while (1) {
        read_dht11();
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}
