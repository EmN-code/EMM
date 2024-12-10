#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "dht_params.h"
#include "dht.h"
#include "event/thread.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "thread.h"

#define DHT_PIN GPIO_PIN(0, 2)

int main(void) {
    printf("Starting DHT11 sensor reading \n");
    
    dht_t dev;
    if (dht_init(&dev, &dht_params[DHT11], DHT_PIN) != 0) {
        printf("Error: failed to initialize DHT11 sensor\n");
        return -1;
    }

    while (1) {
        int16_t temperature, humidity;
        if (dht_read(&dev, &temperature, &humidity) != 0) {
            printf("Error: failed to read from DHT11 sensor\n");
        } else {
            printf("Temperature: %d.%d C, Humidity: %d.%d %%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
        }

        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}