#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "paho_mqtt.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "MQTTClient.h"
#include "thread.h"

#define DHT_PIN GPIO_PIN(PORT_A, 0)  // Example GPIO pin
#define LED_PIN GPIO_PIN(PORT_A, 1)  // Example GPIO pin
#define TEMP_THRESHOLD 30  // Temperature threshold in degrees Celsius

void init_gpio(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);
}

void check_temperature(void) {
    dht_t dev;
    int16_t temperature, humidity;
    
    dht_params_t params = DHT_PARAMS;
    params.pin = DHT_PIN;
    dht_init(&dev, &params);

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, abs(temperature % 10), humidity / 10, abs(humidity % 10));
        } else {
            printf("DHT reading failed\n");
        }
        xtimer_sleep(5);
    }
}

int main(void) {
    init_gpio();
    check_temperature();
    return 0;
}