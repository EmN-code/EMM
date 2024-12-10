
#include "dht_params.h"
#include "periph/i2c.h"
#include "thread.h"
#include "periph/rtc.h"
#include "event/thread.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define SLEEP_DURATION (5 * 1000000U)  // 5 seconds in microseconds

static dht_t dht_dev;

void read_dht(void) {
    int16_t temp, hum;
    int res = dht_read(&dht_dev, &temp, &hum);

    if (res == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
               temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        puts("Failed to read from DHT sensor");
    }
}

int main(void) {
    dht_params_t params = {
        .pin = DHT_PIN,
        .type = DHT11
    };

    if (dht_init(&dht_dev, &params) != DHT_OK) {
        puts("DHT11 initialization failed");
        return 1;
    }

    while (1) {
        read_dht();
        ztimer_sleep(ZTIMER_USEC, SLEEP_DURATION);
    }

    return 0;
}
