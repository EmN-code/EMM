#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "thread_config.h"
#include "thread.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdio.h"
#include "stdbool.h"
#include "sched.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "mma8x5x_params.h"
#include "MQTTClient.h"
#include "unistd.h"

#define THREAD_STACKSIZE_MAIN (1024)
#define THREAD_PRIORITY_MAIN  (THREAD_PRIORITY_MAIN - 1)

char stack1[THREAD_STACKSIZE_MAIN];
char stack2[THREAD_STACKSIZE_MAIN];

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

void *thread_handler(void *arg) {
    (void)arg;
    while (1) {
        // Placeholder for thread task
        ztimer_sleep(ZTIMER_MSEC, 1000); // Sleep for 1 second
    }
    return NULL;
}

int main(void) {
    puts("Starting RIOT application");

    // Create two threads
    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0,
                                thread_handler, NULL, "thread1");

    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1, 0,
                                thread_handler, NULL, "thread2");

    if (thread1_pid < 0 || thread2_pid < 0) {
        puts("Thread creation failed");
        return 1;
    }

    // Change priority of thread1 at runtime
    ztimer_sleep(ZTIMER_MSEC, 5000); // Allow threads to run for a while
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 2);

    while(1) {
        // Placeholder for main function task
    }
    return 0;
}
