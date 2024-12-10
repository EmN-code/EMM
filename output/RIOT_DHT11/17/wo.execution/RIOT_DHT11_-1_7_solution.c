#include "periph/i2c.h"
#include "msg.h"
#include "time_units.h"
#include "thread.h"
#include "periph/rtc.h"
#include "arpa/inet.h"
#include "stdbool.h"
#include "kernel_defines.h"
#include "event/periodic.h"
#include "event/timeout.h"
#include "stdio.h"
#include "MQTTClient.h"
#include "evtimer_msg.h"
#include "dht_params.h"
#include "xtimer.h"
#include "dht.h"
#include "periph/gpio.h"
#include "paho_mqtt.h"
#include "ztimer.h"
#include "stdlib.h"
#include "stddef.h"
#include "mbox.h"
#include "event/thread.h"
#include "timex.h"
#include "netinet/in.h"
#include "mma8x5x.h"
#include "unistd.h"
#include "mma8x5x_params.h"
#include "evtimer_mbox.h"
#include "mma8x5x_regs.h"
#include "stdatomic.h"
#include "thread_config.h"

// Device configuration constants
#define DHT11_PIN GPIO_PIN(0, 2)  // GPIO 2
#define SLEEP_DURATION (5U * US_PER_SEC)  // 5 seconds

// Main function
int main(void)
{
    dht_t dev;
    int16_t temperature, humidity;

    // Initialize DHT11 sensor
    if (dht_init(&dev, DHT11_PIN) != 0) {
        printf("Error initializing DHT sensor\n");
        return -1;
    }

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temperature, &humidity) != 0) {
            printf("Error reading from DHT sensor\n");
        } else {
            printf("Temperature: %0.1f°C, Humidity: %0.1f%%\n", temperature / 10.0, humidity / 10.0);
        }

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_USEC, SLEEP_DURATION);
    }

    return 0;
}