#include "thread_arch.h"
#include "xtimer.h"
#include "timex.h"
#include "dht.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define SAMPLE_INTERVAL (5U)

static dht_t dev;
static dht_params_t params = {
    .pin = DHT_PIN,
    .type = DHT11
};

static void read_dht(void) {
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) == 0) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
    }
    else {
        printf("Failed to read data from DHT11 sensor\n");
    }
}

int main(void) {
    if (dht_init(&dev, &params) != 0) {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }
    while (1) {
        read_dht();
        xtimer_sleep(SAMPLE_INTERVAL);
    }
    return 0;
}