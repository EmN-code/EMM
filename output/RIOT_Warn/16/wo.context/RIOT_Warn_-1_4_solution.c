#include <xtimer.h>
#include <periph/gpio.h>
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 1)
#define THRESHOLD_TEMP 30

static gpio_t led = GPIO_PIN(PORT_B, 0);

static int16_t read_temperature(void) {
    int16_t temp, hum;
    if (dht_read(&dht_params[0], &temp, &hum) != DHT_OK) {
        return -1;
    }
    return temp;
}

int main(void) {
    gpio_init(led, GPIO_OUT);
    dht_params_t dht_params[] = {DHT_PARAM_INIT(DHT_PIN, DHT11)};
    
    while (1) {
        int16_t temperature = read_temperature();
        if (temperature >= 0) {
            if (temperature > THRESHOLD_TEMP) {
                gpio_set(led);
            } else {
                gpio_clear(led);
            }
        }
        xtimer_sleep(5);
    }
    return 0;
}