#include "xtimer.h"
#include "thread.h"
#include "periph/gpio.h"
#include "dht.h"
#include "dht_params.h"

#define TEMP_THRESHOLD 30
#define INTERVAL (5U * US_PER_SEC)

static dht_t dev;

static void read_dht_and_toggle_led(void)
{
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) == 0) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
               temp / 10, temp % 10, hum / 10, hum % 10);
        if (temp > TEMP_THRESHOLD * 10) { // Temperature is returned as degrees Celsius * 10
            gpio_set(GPIO_PIN(0, 0));  // Assuming LED is connected to GPIO0
        }
        else {
            gpio_clear(GPIO_PIN(0, 0));
        }
    }
    else {
        printf("Failed to read data from DHT sensor\n");
    }
}

int main(void)
{
    dht_params_t params = {
        .pin = GPIO_PIN(0, 1),  // Assuming DHT sensor data pin is connected to GPIO1
        .type = DHT11
    };
    dht_init(&dev, &params);

    gpio_init(GPIO_PIN(0, 0), GPIO_OUT);

    while (1) {
        read_dht_and_toggle_led();
        xtimer_sleep(5);
    }

    return 0;
}