#include "ztimer.h"
#include "unistd.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "paho_mqtt.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "msg.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "dht.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "MQTTClient.h"

#define DHT_PIN GPIO_PIN(0, 2) // Define the GPIO pin for the DHT11 sensor

int main(void) {
    // Device descriptor
    dht_t dev;
    int16_t temp, hum;
    int res;

    // Initialize GPIO
    gpio_init(DHT_PIN, GPIO_IN);

    // Initialize DHT sensor
    if (dht_init(&dev, DHT_PIN, DHT11) != 0) {
        printf("DHT initialization failed\n");
        return 1;
    }

    while (1) {
        // Read temperature and humidity
        res = dht_read(&dev, &temp, &hum);
        if (res == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}