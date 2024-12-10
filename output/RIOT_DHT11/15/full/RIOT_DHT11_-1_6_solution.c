#include "ztimer.h"
#include "timex.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stdio.h"
#include "unistd.h"
#include "thread.h"
#include "time_units.h"
#include "timex.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "stdio.h"

#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "kernel_defines.h"

#include "dht_params.h"
#include "dht.h"

#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"

#define DHT_PIN GPIO_PIN(0, 2)

static dht_t dev;

void *dht_thread(void *arg) {
    int16_t temperature, humidity;

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return NULL;
}

int main(void) {
    if (dht_init(&dev, &dht_params[0]) != 0) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    // Create a thread to read sensor data every five seconds
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, dht_thread, NULL, "DHT Thread");

    return 0;
}