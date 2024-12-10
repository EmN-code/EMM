#include "ztimer.h"
#include "stdlib.h"
#include "stdbool.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "sys/socket.h"
#include "stddef.h"
#include "stdio.h"
#include "stdatomic.h"
#include "pthread.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_system.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "architecture.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"

static char stack[THREAD_STACKSIZE_MAIN];

void* dht11_reader(void* arg) {
    (void)arg;
    dht_t dev;
    int16_t temp, hum;

    while (1) {
        int res = dht_read(&dev, &temp, &hum);
        if (res == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read DHT sensor\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, dht11_reader, NULL, "dht_reader");
    if (pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create DHT11 reader thread\n");
        return 1;
    }

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}