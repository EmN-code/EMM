
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "esp_system.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "mutex.h"
#include "msg.h"
#include "dht.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "net/sock/util.h"
#include "MQTTClient.h"
#include "stdlib.h"
#include "net/sock/udp.h"
#include "sys/socket.h"
#include "mbox.h"
#include "stdio.h"
#include "sched.h"
#include "xtimer.h"
#include "thread.h"
#include "arpa/inet.h"
#include "mma8x5x_regs.h"
#include "unistd.h"
#include "stdatomic.h"
#include "net/emcute.h"
#include "mma8x5x.h"
#include "esp_wifi.h"
#include "dht_params.h"
#include "mma8x5x_params.h"

#define STACKSIZE 512
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

void thread_func_high(void *arg)
{
    (void)arg;
    printf("High priority thread running\n");
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
}

void thread_func_low(void *arg)
{
    (void)arg;
    printf("Low priority thread running\n");
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
}

int main(void)
{
    char stack_high[STACKSIZE];
    char stack_low[STACKSIZE];

    kernel_pid_t pid_high = thread_create(stack_high, sizeof(stack_high), PRIORITY_HIGH, 0, thread_func_high, NULL, "thread_high");
    kernel_pid_t pid_low = thread_create(stack_low, sizeof(stack_low), PRIORITY_LOW, 0, thread_func_low, NULL, "thread_low");

    printf("Threads created: High priority thread PID: %d, Low priority thread PID: %d\n", pid_high, pid_low);

    ztimer_sleep(ZTIMER_MSEC, 5000);

    printf("Changing priority of high priority thread to low\n");
    sched_change_priority(pid_high, PRIORITY_LOW);

    ztimer_sleep(ZTIMER_MSEC, 5000);

    printf("Changing priority of low priority thread to high\n");
    sched_change_priority(pid_low, PRIORITY_HIGH);

    return 0;
}
