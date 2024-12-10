
#include "esp_system.h"
#include "paho_mqtt.h"
#include "net/gcoap.h"
#include "periph/rtc.h"
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
#include "esp_wifi.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "mma8x5x_params.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static dht_t dht_dev;

void read_dht11(void) {
    int16_t temp, hum;
    if (dht_read(&dht_dev, &temp, &hum) == 0) {
        printf("Temperature: %d, Humidity: %d\n", temp, hum);
    } else {
        printf("Failed to read from DHT11 sensor\n");
    }
}

int main(void) {
    // Initialize DHT11 sensor
    dht_params_t dht_params = { .pin = DHT_PIN };
    if (dht_init(&dht_dev, &dht_params) != 0) {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    while (1) {
        read_dht11();
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}
