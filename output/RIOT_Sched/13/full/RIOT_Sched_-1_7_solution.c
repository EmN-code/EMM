#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "unistd.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdio.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "stdbool.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "sched.h"
#include "paho_mqtt.h"
#include "mutex.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "arpa/inet.h"
#include "MQTTClient.h"
#include "thread.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN + THREAD_EXTRA_STACKSIZE_PRINTF)

char stack1[THREAD_STACKSIZE];
char stack2[THREAD_STACKSIZE];

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

void *thread_1_func(void *arg) {
    (void)arg; // to avoid unused variable
    while (1) {
        // Simulated work of the thread
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_2_func(void *arg) {
    (void)arg; // to avoid unused variable
    while (1) {
        // Simulated work of the thread
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    // Create and start two threads
    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, thread_1_func, NULL, "thread_1");
    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1, 0, thread_2_func, NULL, "thread_2");

    // Change priorities at runtime
    xtimer_sleep(5); // wait for some time before changing priorities
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN - 3);

    while (1) {
        // main thread loop
        xtimer_sleep(10);
    }
    return 0;
}