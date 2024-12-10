#include "stdint.h"
#include "unistd.h"
#include "thread_arch.h"
#include "thread_config.h"
#include "stdio.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "sched.h"
#include "mbox.h"
#include "msg.h"
#include "netinet/in.h"
#include "MQTTClient.h"
#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "thread.h"
#include "paho_mqtt.h"
#include "architecture.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "kernel_defines.h"
#include "arpa/inet.h"

#define THREAD_STACKSIZE_DEFAULT 1024
#define THREAD_PRIORITY_MAIN -1

void *thread1_function(void *arg) {
    while (1) {
        printf("Thread 1 is running\n");
        sleep(2);
    }
    return NULL;
}

void *thread2_function(void *arg) {
    while (1) {
        printf("Thread 2 is running\n");
        sleep(2);
    }
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];

    kernel_pid_t thread1_pid = thread_create(stack1, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1, 0, thread1_function, NULL, "thread1");
    kernel_pid_t thread2_pid = thread_create(stack2, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1, 0, thread2_function, NULL, "thread2");

    if (thread1_pid < 0 || thread2_pid < 0) {
        printf("Thread creation failed\n");
        return -1;
    }

    sleep(5);

    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN - 2);

    while (1) {
        sleep(1);
    }

    return 0;
}