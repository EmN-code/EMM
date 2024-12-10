
#include "mma8x5x.h"
#include "periph/rtc.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "stdio.h"
#include "unistd.h"
#include "MQTTClient.h"
#include "stddef.h"
#include "time_units.h"
#include "stdatomic.h"
#include "paho_mqtt.h"
#include "stdlib.h"
#include "periph/i2c.h"
#include "event/thread.h"
#include "periph/gpio.h"
#include "mma8x5x_params.h"
#include "dht_params.h"
#include "esp_system.h"
#include "ztimer.h"
#include "netinet/in.h"
#include "esp_wifi.h"
#include "xtimer.h"
#include "msg.h"
#include "timex.h"
#include "net/sock/udp.h"
#include "evtimer_mbox.h"
#include "mutex.h"
#include "thread.h"
#include "mbox.h"
#include "sys/socket.h"
#include "mma8x5x_regs.h"
#include "dht.h"
#include "stdbool.h"
#include "evtimer_msg.h"
#include "net/sock/util.h"
#include "arpa/inet.h"
#include "kernel_defines.h"

#define DHT_PIN GPIO_PIN(0, 2)

static dht_t dht_dev;

int main(void) {
    int16_t temperature, humidity;

    // Initialize the DHT11 sensor
    dht_dev.pin = DHT_PIN;
    dht_dev.type = DHT11;

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dht_dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        } else {
            puts("Failed to read from DHT11 sensor");
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}
