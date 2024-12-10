#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "time_units.h"
#include "timex.h"
#include "thread_arch.h"
#include "thread.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "sched.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "mutex.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "architecture.h"

#define STACKSIZE_THREAD (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY_THREAD1 (THREAD_PRIORITY_MAIN + 1)
#define PRIORITY_THREAD2 (THREAD_PRIORITY_MAIN + 2)

char thread_stack1[STACKSIZE_THREAD];
char thread_stack2[STACKSIZE_THREAD];

void *thread_func1(void *arg) {
    printf("Thread 1 running with initial priority\n");
    while (1) {
        // Simulate work by sleeping
        usleep(500000);
    }
    return NULL;
}

void *thread_func2(void *arg) {
    printf("Thread 2 running with initial priority\n");
    while (1) {
        // Simulate work by sleeping
        usleep(500000);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid_thread1, pid_thread2;

    // Create threads
    pid_thread1 = thread_create(thread_stack1, STACKSIZE_THREAD, PRIORITY_THREAD1, 0, thread_func1, NULL, "thread1");
    pid_thread2 = thread_create(thread_stack2, STACKSIZE_THREAD, PRIORITY_THREAD2, 0, thread_func2, NULL, "thread2");

    // Allow some time to pass
    usleep(2000000);

    // Change the priority of the threads
    sched_change_priority(thread_get(pid_thread1), THREAD_PRIORITY_MAIN + 3);
    sched_change_priority(thread_get(pid_thread2), THREAD_PRIORITY_MAIN + 4);

    return 0;
}