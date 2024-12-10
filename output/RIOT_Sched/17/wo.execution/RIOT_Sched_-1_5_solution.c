#include <unistd.h>
#include <time_units.h>
#include <thread_config.h>
#include <thread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <sched.h>

static char stack1[THREAD_STACKSIZE_DEFAULT];
static char stack2[THREAD_STACKSIZE_DEFAULT];

void *thread_func(void *arg) {
    int thread_no = *((int *)arg);
    while (1) {
        printf("Thread %d is running\n", thread_no);
        usleep(10000); // sleep for 10 milliseconds
    }
    return NULL;
}

int main(void) {
    int thread1_no = 1;
    int thread2_no = 2;

    kernel_pid_t thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                                          0, thread_func, &thread1_no, "thread1");

    kernel_pid_t thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                                          0, thread_func, &thread2_no, "thread2");

    // Change priority of thread1
    sched_change_priority(thread_get(thread1), THREAD_PRIORITY_MAIN - 3);
    
    usleep(5000000); // sleep for 5 seconds to observe thread running

    // Change priority of thread2
    sched_change_priority(thread_get(thread2), THREAD_PRIORITY_MAIN - 4);

    while (1) {
        pause(); // keep main thread running to allow other threads to operate
    }

    return 0;
}