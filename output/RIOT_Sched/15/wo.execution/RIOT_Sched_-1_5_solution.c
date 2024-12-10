#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "thread_config.h"
#include "thread_arch.h"
#include "thread.h"
#include "sched.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "architecture.h"

#define STACK_SIZE 1024
#define THREAD_PRIORITY_1 5
#define THREAD_PRIORITY_2 10

char stack1[STACK_SIZE];
char stack2[STACK_SIZE];
kernel_pid_t thread1;
kernel_pid_t thread2;

void *thread_func(void *arg) {
    (void)arg;
    while (1) {
        // Simulate some work
    }
    return NULL;
}

int main(void) {
    // Create two threads
    thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_1, 0,
                            thread_func, NULL, "thread1");
    thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_2, 0,
                            thread_func, NULL, "thread2");

    // Changing priority of the threads at runtime
    thread_sleep();
    sched_change_priority(thread_get(thread1), THREAD_PRIORITY_2);
    sched_change_priority(thread_get(thread2), THREAD_PRIORITY_1);

    // Return from main
    return 0;
}