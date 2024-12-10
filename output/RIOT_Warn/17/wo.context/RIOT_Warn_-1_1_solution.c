#include "xtimer.h"
#include "timex.h"
#include "dht.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(PORT_A, 10)
#define LED_PIN GPIO_PIN(PORT_B, 5)
#define THRESHOLD_TEMP 30

int main(void)
{
    dht_t dev;
    int16_t temp, hum;
    
    if (dht_init(&dev, &(dht_params_t){.pin = DHT_PIN, .type = DHT11}) != 0) {
        puts("DHT initialization failed");
        return 1;
    }

    gpio_init(LED_PIN, GPIO_OUT);
    
    while (1) {
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum);

            if (temp > THRESHOLD_TEMP * 10) { // DHT gives temp in decicelsius
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            puts("Failed to read DHT sensor");
        }

        xtimer_sleep(5);
    }

    return 0;
}