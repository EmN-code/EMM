
#include "dht_params.h"
#include "periph/i2c.h"
#include "thread.h"
#include "periph/rtc.h"
#include "event/thread.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN             GPIO_PIN(0, 2)
#define DHT_TYPE            DHT11
#define SLEEP_INTERVAL      (5 * 1000 * 1000U)  // 5 seconds in microseconds

static dht_t dht_dev;

void read_dht(void) {
    int16_t temp, hum;
    if (dht_read(&dht_dev, &temp, &hum) == 0) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        puts("Failed to read from DHT sensor");
    }
}

int main(void) {
    dht_params_t params = {
        .pin = DHT_PIN,
        .type = DHT_TYPE
    };

    if (dht_init(&dht_dev, &params) != 0) {
        puts("Failed to initialize DHT sensor");
        return 1;
    }

    while (1) {
        read_dht();
        ztimer_sleep(ZTIMER_USEC, SLEEP_INTERVAL);
    }

    return 0;
}
