#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "timex.h"
#include "time_units.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "msg.h"
#include "mbox.h"
#include "mutex.h"
#include "xtimer.h"
#include "ztimer.h"
#include "thread.h"
#include "thread_config.h"
#include "sched.h"
#include "unistd.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_system.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "arpa/inet.h"
#include "netinet/in.h"

void *thread_function1(void *arg) {
    (void)arg;
    uint32_t count = 0;
    while (1) {
        printf("Thread 1 count: %lu\n", count++);
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_function2(void *arg) {
    (void)arg;
    uint32_t count = 0;
    while (1) {
        printf("Thread 2 count: %lu\n", count++);
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_MAIN];
    char stack2[THREAD_STACKSIZE_MAIN];
    kernel_pid_t thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0,
                                         thread_function1, NULL, "thread1");
    kernel_pid_t thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, 0,
                                         thread_function2, NULL, "thread2");

    xtimer_sleep(5);

    // Change priority of the threads
    printf("Changing priority of thread 1\n");
    sched_change_priority(thread_get(thread1), THREAD_PRIORITY_MAIN - 3);

    printf("Changing priority of thread 2\n");
    sched_change_priority(thread_get(thread2), THREAD_PRIORITY_MAIN - 4);

    return 0;
}