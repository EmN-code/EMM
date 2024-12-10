#include "ztimer.h"
#include "xtimer.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "stddef.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "timex.h"
#include "time_units.h"
#include "thread_flags.h"
#include "thread.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "msg.h"
#include "mut.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "esp_system.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"

#define DHT11_PIN GPIO_PIN(0, 2)

static dht_t dht_dev;
static dht_params_t dht_params = {
    .pin = DHT11_PIN,
    .type = DHT_TYPE_DHT11
};

void *dht_read_thread(void *arg) {
    (void)arg;
    int16_t temperature, humidity;
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    if (dht_init(&dht_dev, &dht_params) != DHT_OK) {
        printf("Error initializing DHT11\n");
        return NULL;
    }

    while (true) {
        if (dht_read(&dht_dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
        } else {
            printf("Error reading DHT11\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];

    kernel_pid_t pid = thread_create(
        stack,
        sizeof(stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        dht_read_thread,
        NULL,
        "dht_read"
    );

    if (pid > 0) {
        printf("Successfully started DHT reading thread\n");
    } else {
        printf("Failed to create DHT reading thread\n");
    }

    return 0;
}