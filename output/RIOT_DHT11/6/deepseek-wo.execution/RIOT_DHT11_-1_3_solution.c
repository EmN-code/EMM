
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "dht.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "dht_params.h"
#include "thread.h"

#define DHT_PIN GPIO_PIN(0, 2)

static dht_t dht_dev;

void delay(uint32_t milliseconds) {
    for (uint32_t i = 0; i < milliseconds * 1000; i++) {
        __asm__("nop");
    }
}

int main(void) {
    int16_t temperature, humidity;

    // Initialize DHT11 device
    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT11
    };
    dht_init(&dht_dev, &dht_params);

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dht_dev, &temperature, &humidity) == 0) {
            // Print temperature and humidity
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        // Delay for 5 seconds
        delay(5000);
    }

    return 0;
}
