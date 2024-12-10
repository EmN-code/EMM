#include <ztimer.h>
#include <stdio.h>
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "periph/rtc.h"
#include "dht.h"
#include "dht_params.h"
#include "MQTTClient.h"
#include "thread.h"
#include "event/thread.h"

#define DHT_PIN GPIO_PIN(0, 2)

int main(void) {
    dht_params_t dht_params;
    dht_params.pin = DHT_PIN;
    dht_params.type = DHT11;
    uint16_t temperature, humidity;

    printf("DHT11 test application
");

    while (1) {
        if (dht_read(&dht_params, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C
", temperature / 10, temperature % 10);
            printf("Humidity: %d.%d%%
", humidity / 10, humidity % 10);
        } else {
            printf("Failed to read from DHT sensor
");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);  // sleep for 5 seconds
    }

    return 0;
}