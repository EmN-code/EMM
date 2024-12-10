
#include <stdio.h>
#include <xtimer.h>
#include <dht.h>
#include <periph/gpio.h>

#define DHT_PIN GPIO_PIN(0, 2)  // GPIO 2 on ESP32
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static dht_t dht11;

void read_dht11(void) {
    int16_t temp, hum;
    int res = dht_read(&dht11, &temp, &hum);

    if (res == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        printf("Error reading DHT11 sensor: %d\n", res);
    }
}

int main(void) {
    dht_params_t params = {
        .pin = DHT_PIN,
        .type = DHT11,
    };

    if (dht_init(&dht11, &params) != DHT_OK) {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    while (1) {
        read_dht11();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
