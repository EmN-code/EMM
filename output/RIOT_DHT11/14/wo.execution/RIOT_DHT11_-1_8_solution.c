#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <dht.h>
#include <periph/gpio.h>
#include <msg.h>
#include <thread.h>
#include <mutex.h>
#include <timex.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <esp_system.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <kernel_defines.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <dht_params.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include "periph/gpio.h"

#define DHT11_PIN GPIO_NUM_2
#define THREAD_STACKSIZE_MAIN THREAD_STACKSIZE_DEFAULT
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)

static char stack[THREAD_STACKSIZE_MAIN];

static void *dht11_thread(void *arg) {
    dht_t dev;
    int16_t temp, hum;
    dht_params_t params;

    params.pin = DHT11_PIN;
    params.type = DHT11;

    // Initialize the sensor
    if (dht_init(&dev, &params) == 0) {
        while (1) {
            if (dht_read(&dev, &temp, &hum) == 0) {
                printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                        temp / 10, temp % 10, hum / 10, hum % 10);
            } else {
                printf("Failed to read data from DHT11 sensor\n");
            }
            xtimer_sleep(5);
        }
    } else {
        printf("Failed to initialize DHT11 sensor\n");
    }
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0,
                  dht11_thread, NULL, "dht11_thread");
    return 0;
}