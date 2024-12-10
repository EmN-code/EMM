#include "ztimer.h"
#include "time_units.h"
#include "thread.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdatomic.h"
#include "periph/gpio.h"
#include "netinet/in.h"
#include "msg.h"
#include "mutex.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "ztimer.h"
#include "ztimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include "xtimer.h"
#include <dht.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <event.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <mbox.h>
#include <mmutex.h>
#include <mqttClient.h>
#include <mutex.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <periph/gpio.h>
#include <paho_mqtt.h>
#include <periph/i2c.h>
#include <periph/rtc.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/socket.h>
#include <thread.h>
#include <timex.h>
#include <time_units.h>
#include <unistd.h>

#define DHT_PIN GPIO_PIN(0, 2)
#define LOOP_INTERVAL (5 * US_PER_SEC)

static kernel_pid_t dht_thread_pid;
static char dht_thread_stack[THREAD_STACKSIZE_MAIN];

void *dht_thread(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temp, hum;

    while (1) {
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return NULL;
}

int main(void) {

    dht_thread_pid = thread_create(
        dht_thread_stack,
        sizeof(dht_thread_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        dht_thread,
        NULL,
        "dht_thread");

    printf("DHT reading thread started\n");

    return 0;
}