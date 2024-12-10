#include <ztimer.h>
#include <unistd.h>
#include <timex.h>
#include <thread.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <dht_params.h>
#include <dht.h>
#include <time_units.h>
#include <MQTTClient.h>

#define DHT_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5U * US_PER_SEC)

int main(void) {
    dht_t dev;
    if (dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN) != 0) {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    int16_t temperature, humidity;

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
        } else {
            printf("Failed to read data from sensor\n");
        }
        ztimer_sleep(ZTIMER_MSEC, SLEEP_INTERVAL);
    }

    return 0;
}