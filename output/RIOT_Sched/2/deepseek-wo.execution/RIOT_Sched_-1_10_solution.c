
#include "thread.h"
#include "sched.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "net/sock/udp.h"
#include "timex.h"
#include "unistd.h"
#include "stddef.h"
#include "string.h"
#include "architecture.h"
#include "mma8x5x_params.h"
#include "MQTTClient.h"
#include "dht_params.h"
#include "xtimer.h"
#include "mma8x5x_regs.h"
#include "periph/i2c.h"
#include "paho_mqtt.h"
#include "ztimer.h"
#include "mma8x5x.h"
#include "net/emcute.h"
#include "periph/rtc.h"
#include "freertos/FreeRTOS.h"
#include "kernel_defines.h"
#include "shell.h"
#include "periph/gpio.h"
#include "stdlib.h"
#include "time_units.h"
#include "nvs_flash.h"
#include "stdbool.h"
#include "esp_system.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "dht.h"
#include "esp_wifi.h"
#include "net/gcoap.h"
#include "mutex.h"
#include "netinet/in.h"
#include "freertos/task.h"
#include "nvs.h"
#include "stdatomic.h"
#include "net/sock/util.h"

#define THREAD_STACKSIZE_DEFAULT (512)

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running at priority %d\n", thread_get_priority(thread_get_active()));
        thread_yield();
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running at priority %d\n", thread_get_priority(thread_get_active()));
        thread_yield();
    }
    return NULL;
}

int main(void)
{
    thread1_pid = thread_create(
        (char *)malloc(THREAD_STACKSIZE_DEFAULT),
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        thread1_func,
        NULL,
        "thread1"
    );

    thread2_pid = thread_create(
        (char *)malloc(THREAD_STACKSIZE_DEFAULT),
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 2,
        THREAD_CREATE_STACKTEST,
        thread2_func,
        NULL,
        "thread2"
    );

    xtimer_sleep(5);

    sched_change_priority(thread_get_from_pid(thread1_pid), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get_from_pid(thread2_pid), THREAD_PRIORITY_MAIN - 1);

    return 0;
}
