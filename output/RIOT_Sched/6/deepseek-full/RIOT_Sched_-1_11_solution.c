
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

#define STACKSIZE 1024
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

void thread_func_1(void *arg) {
    (void)arg;
    printf("Thread 1 running\n");
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
}

void thread_func_2(void *arg) {
    (void)arg;
    printf("Thread 2 running\n");
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
}

int main(void) {
    char stack_1[STACKSIZE];
    char stack_2[STACKSIZE];

    kernel_pid_t pid_1 = thread_create(stack_1, sizeof(stack_1), PRIORITY_LOW, 0, thread_func_1, NULL, "thread_1");
    kernel_pid_t pid_2 = thread_create(stack_2, sizeof(stack_2), PRIORITY_LOW, 0, thread_func_2, NULL, "thread_2");

    printf("Threads created\n");

    ztimer_sleep(ZTIMER_SEC, 5);
    sched_change_priority(pid_1, PRIORITY_HIGH);
    printf("Changed priority of thread 1 to HIGH\n");

    ztimer_sleep(ZTIMER_SEC, 5);
    sched_change_priority(pid_2, PRIORITY_HIGH);
    printf("Changed priority of thread 2 to HIGH\n");

    return 0;
}
