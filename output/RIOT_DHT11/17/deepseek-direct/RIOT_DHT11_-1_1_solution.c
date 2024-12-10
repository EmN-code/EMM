
#include <stdio.h>
#include "xtimer.h"
#include "dht.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(0, 2)  // GPIO 2 on ESP32
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void read_dht11(void) {
    int16_t temp, hum;
    if (dht_read(DHT_TYPE_DHT11, DHT_PIN, &hum, &temp) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
               temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        puts("Failed to read from DHT11 sensor");
    }
}

int main(void) {
    puts("DHT11 sensor test");

    // Initialize the DHT11 sensor
    if (gpio_init(DHT_PIN, GPIO_OUT) < 0) {
        puts("Failed to initialize GPIO");
        return 1;
    }

    while (1) {
        read_dht11();
        xtimer_sleep(INTERVAL / 1000000U); // Convert microseconds to seconds
    }

    return 0;
}
