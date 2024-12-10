#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <thread_arch.h>
#include <thread_config.h>
#include <thread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stddef.h>
#include <sched.h>
#include <paho_mqtt.h>
#include <netinet/in.h>
#include <msg.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <architecture.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <MQTTClient.h>  // Provided all necessary data types are declared or autocompleted
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>

#define STACK_SIZE            (1024)
#define THREAD_PRIORITY_MAIN  (THREAD_PRIORITY_MAIN + 1)
#define THREAD_PRIORITY_HELPER (THREAD_PRIORITY_MAIN + 2)

static char stack0[STACK_SIZE];
static char stack1[STACK_SIZE];

// Function for the first thread
void *thread1_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

// Function for the second thread
void *thread2_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    printf("Starting threads...\n");

    // Create the first thread
    kernel_pid_t thread1_pid = thread_create(stack0, STACK_SIZE, THREAD_PRIORITY_MAIN,
                                             THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");

    // Create the second thread
    kernel_pid_t thread2_pid = thread_create(stack1, STACK_SIZE, THREAD_PRIORITY_HELPER,
                                             THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    // Main thread sleeping
    xtimer_sleep(3);

    // Change the priority of threads at runtime
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_HELPER);
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN);

    return 0;
}