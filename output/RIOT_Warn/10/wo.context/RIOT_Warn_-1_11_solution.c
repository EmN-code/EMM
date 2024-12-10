#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 0)
#define LED_PIN GPIO_PIN(PORT_B, 1)
#define TEMP_THRESHOLD 30

static dht_params_t dht_params = {
    .pin = DHT_PIN,
    .type = DHT11,
};

void read_dht(void) {
    int16_t temp, hum;
    if (dht_read(&dht_params, &temp, &hum) != 0) {
        puts("DHT reading failed");
        return;
    }
    printf("Temperature: %d°C\n", temp);

    if (temp > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
    } else {
        gpio_clear(LED_PIN);
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    dht_init(&dht_params);

    while (1) {
        read_dht();
        xtimer_sleep(5);
    }

    return 0;
}