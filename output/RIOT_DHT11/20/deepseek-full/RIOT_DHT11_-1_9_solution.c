
#include "esp_system.h"
#include "stddef.h"
#include "ztimer.h"
#include "stdbool.h"
#include "mutex.h"
#include "periph/i2c.h"
#include "msg.h"
#include "dht.h"
#include "evtimer_mbox.h"
#include "kernel_defines.h"
#include "timex.h"
#include "periph/gpio.h"
#include "netinet/in.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "net/sock/util.h"
#include "dht_params.h"
#include "stdlib.h"
#include "net/sock/udp.h"
#include "sys/socket.h"
#include "mbox.h"
#include "stdio.h"
#include "arpa/inet.h"
#include "xtimer.h"
#include "thread.h"
#include "mma8x5x_regs.h"
#include "net/emcute.h"
#include "unistd.h"
#include "stdatomic.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "mma8x5x_params.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5 * US_PER_SEC)

static dht_t dht_dev;

int main(void)
{
    int16_t temperature, humidity;

    /* Initialize the DHT11 sensor */
    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT11
    };
    dht_init(&dht_dev, &dht_params);

    while (1) {
        /* Read temperature and humidity */
        if (dht_read(&dht_dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        /* Sleep for 5 seconds */
        ztimer_sleep(ZTIMER_USEC, SLEEP_INTERVAL);
    }

    return 0;
}
