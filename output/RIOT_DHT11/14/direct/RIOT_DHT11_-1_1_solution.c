#include <ztimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(0, 2)
#define INTERVAL (5LU * US_PER_SEC) // 5 seconds in microseconds

static dht_t dev;

static void read_dht11(void) {
    int16_t temp, hum;
    int res = dht_read(&dev, &temp, &hum);
    if (res == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, abs(temp % 10), hum / 10, hum % 10);
    }
    else {
        printf("Error reading DHT11
");
    }
}

int main(void) {
    int res = dht_init(&dev, DHT11, DHT_PIN);
    if (res != DHT_OK) {
        printf("Failed to initialize DHT sensor
");
        return 1;
    }

    printf("Initialized DHT11 on GPIO 2\n");

    while (1) {
        read_dht11();
        ztimer_sleep(ZTIMER_SEC, INTERVAL);
    }

    return 0;
}