#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <event/thread.h>
#include <dht.h>

#define TEMP_THRESHOLD 30 // Temperature threshold in degree Celsius
#define DHT_PIN GPIO_PIN(PORT_A, 5)  // Assumed pin for DHT
#define LED_PIN GPIO_PIN(PORT_B, 0)  // Assumed pin for LED

void *read_sensor(void *arg) {
    dht_t dev;
    int16_t temperature, humidity;
    dht_params_t params;
    params.pin = DHT_PIN;

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d°C, Humidity: %d\n", temperature, humidity);

            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        thread_sleep(5);
    }

    return NULL;
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    thread_create(NULL, THREAD_STACKSIZE_MAIN, THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST, read_sensor, NULL, "read_sensor");

    return 0;
}