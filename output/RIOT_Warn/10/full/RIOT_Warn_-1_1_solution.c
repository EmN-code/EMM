#include <xtimer.h>
#include <periph/gpio.h>
#include <dht.h>

#define LED_PIN     GPIO_PIN(0, 2)
#define DHT_PIN     GPIO_PIN(0, 4)
#define TEMP_THRESHOLD 30  // Temperature threshold in Celsius

dht_t dev;

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);
    
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT11;
    
    if (dht_init(&dev, &params) != 0) {
        puts("DHT initialization failed!");
        return 1;
    }

    while (1) {
        int16_t temperature, humidity;
        int res = dht_read(&dev, &temperature, &humidity);
        if (res != 0) {
            printf("DHT read failed with error code: %d\n", res);
        } else {
            printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        xtimer_sleep(5);
    }

    return 0;
}