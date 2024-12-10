
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)  // GPIO 2 on ESP32
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void read_dht(void) {
    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        puts("Error initializing DHT sensor");
        return;
    }

    if (dht_read(&dev, &temp, &hum) != DHT_OK) {
        puts("Error reading DHT sensor");
        return;
    }

    printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
           temp / 10, temp % 10, hum / 10, hum % 10);
}

int main(void) {
    while (1) {
        read_dht();
        xtimer_sleep(INTERVAL / 1000000);
    }

    return 0;
}
