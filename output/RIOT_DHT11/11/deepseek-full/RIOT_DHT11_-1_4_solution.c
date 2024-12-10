
#include "periph/gpio.h"
#include "ztimer.h"
#include "timex.h"
#include "stdio.h"
#include "stdlib.h"

#define DHT11_PIN GPIO_PIN(0, 2)
#define INTERVAL (5 * US_PER_SEC)

static void read_dht11(void) {
    int16_t temp, hum;
    if (dht_read(DHT_TYPE_DHT11, DHT11_PIN, &temp, &hum) == 0) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        puts("Failed to read from DHT11 sensor");
    }
}

int main(void) {
    if (gpio_init(DHT11_PIN, GPIO_OUT) < 0) {
        puts("Failed to initialize GPIO");
        return 1;
    }

    while (1) {
        read_dht11();
        ztimer_sleep(ZTIMER_USEC, INTERVAL);
    }

    return 0;
}
