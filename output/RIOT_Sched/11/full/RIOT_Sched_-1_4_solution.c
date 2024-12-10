#include <xtimer.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <time_units.h>
#include <timex.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <thread_config.h>
#include <thread.h>
#include <thread_arch.h>
#include <mma8x5x_params.h>
#include <dht_params.h>
#include <unistd.h>
#include <paho_mqtt.h>

// Function signature for tasks executed by the threads.
void *thread_task(void *arg) {
    (void)arg; // to avoid unused parameter warning.
    while (1) {
        // Simulate work with some sleep.
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];

    kernel_pid_t thread1 = thread_create(stack1, sizeof(stack1),
                                         THREAD_PRIORITY_MAIN - 1,
                                         THREAD_CREATE_STACKTEST,
                                         thread_task, NULL, "thread1");

    kernel_pid_t thread2 = thread_create(stack2, sizeof(stack2),
                                         THREAD_PRIORITY_MAIN - 2,
                                         THREAD_CREATE_STACKTEST,
                                         thread_task, NULL, "thread2");
    
    // Change the priority of thread1 at runtime
    uint8_t new_priority = THREAD_PRIORITY_MAIN - 3;
    sched_change_priority(thread_get(thread1), new_priority);

    // Main thread loop.
    while (1) {
        // Perform main thread tasks here and yield control.
        xtimer_sleep(2);
    }

    return 0;
}
