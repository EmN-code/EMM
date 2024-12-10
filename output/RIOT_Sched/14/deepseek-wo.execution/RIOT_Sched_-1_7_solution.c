
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <thread.h>
#include <msg.h>

#define STACKSIZE 1024
#define NUM_THREADS 2

void thread_func(void *arg) {
    (void)arg;
    printf("Thread %d running at priority %d\n", thread_getpid(), thread_get_priority(thread_get_active()));
    while (1) {
        // Thread work
    }
}

int main(void) {
    msg_t msg_array[NUM_THREADS];
    msg_init_queue(msg_array, NUM_THREADS);

    char stacks[NUM_THREADS][STACKSIZE];
    kernel_pid_t pids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        pids[i] = thread_create(stacks[i], STACKSIZE, THREAD_PRIORITY_MAIN + i + 1, 0, thread_func, NULL, "thread");
    }

    // Change priority of the first thread
    sched_change_priority(thread_get(pids[0]), THREAD_PRIORITY_MAIN + 2);

    printf("Main thread running at priority %d\n", thread_get_priority(thread_get_active()));

    return 0;
}
