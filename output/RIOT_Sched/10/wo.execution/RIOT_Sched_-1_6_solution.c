#include <ztimer.h>
#include <unistd.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <sched.h>
#include <thread.h>
#include <msg.h>
#include <event/thread.h>
#include <xtimer.h>
#include <time_units.h>
#include <timex.h>
#include <mbox.h>
#include <kernel_defines.h>

#define THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN)  // Define stack size for threads

char stack1[THREAD_STACKSIZE];
char stack2[THREAD_STACKSIZE];

void *thread_func(void *arg) {
    (void) arg;
    while (1) {
        // Do some work here...
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1, thread2;

    // Create two threads
    thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0,
                            thread_func, NULL, "thread1");

    thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, 0,
                            thread_func, NULL, "thread2");

    xtimer_sleep(5); // Allow threads to run

    // Change priority at runtime
    sched_change_priority(thread_get(thread1), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(thread2), THREAD_PRIORITY_MAIN - 1);

    while (1) {
        // Main loop can perform other tasks...
    }

    return 0;
}
