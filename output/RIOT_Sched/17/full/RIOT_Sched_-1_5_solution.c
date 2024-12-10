#include <ztimer.h> 
#include <xtimer.h> 
#include <unistd.h> 
#include <time_units.h> 
#include <timex.h> 
#include <thread_arch.h> 
#include <thread.h> 
#include <stdlib.h> 
#include <stdatomic.h> 
#include <stddef.h> 
#include <stdio.h> 
#include <stdbool.h> 
#include <sched.h> 
#include <paho_mqtt.h> 
#include <msg.h> 
#include <mbox.h> 
#include <kernel_defines.h> 
#include <ztimer.h> 
#include <architecture.h> 
#include <evtimer_msg.h> 
#include <evtimer_mbox.h> 
#include <event/thread.h> 
#include <MQTTClient.h> 

#define THREAD_STACKSIZE_DEFAULT 1024
#define THREAD_PRIORITY_MAIN 5
#define THREAD_PRIORITY_SECONDARY (THREAD_PRIORITY_MAIN-1)

char stack1[THREAD_STACKSIZE_DEFAULT];
char stack2[THREAD_STACKSIZE_DEFAULT];

void *thread_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread %s is running with priority %d\n", (char *) arg, thread_get_active()->priority);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0, thread_func, "1", "Thread 1");
    kernel_pid_t thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_SECONDARY, 0, thread_func, "2", "Thread 2");

    // Give some time for threads to start
    ztimer_sleep(ZTIMER_SEC, 5);

    // Change priority of second thread
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN);

    return 0;
}