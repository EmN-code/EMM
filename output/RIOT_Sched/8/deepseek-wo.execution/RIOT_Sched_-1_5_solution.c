
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <thread.h>
#include <sched.h>
#include <msg.h>

#define STACKSIZE 512
#define NUM_THREADS 2

void *thread_func(void *arg) {
    kernel_pid_t pid = thread_getpid();
    printf("Thread %d started\n", pid);
    while (1) {
        printf("Thread %d running\n", pid);
        sleep(1);
    }
    return NULL;
}

int main(void) {
    char stacks[NUM_THREADS][STACKSIZE];
    kernel_pid_t pids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        pids[i] = thread_create(stacks[i], STACKSIZE, THREAD_PRIORITY_MAIN - 1, 0, thread_func, NULL, "thread");
        if (pids[i] <= KERNEL_PID_UNDEF) {
            printf("Error creating thread %d\n", i);
            return 1;
        }
    }

    sleep(5);

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_t *thread = thread_get(pids[i]);
        if (thread != NULL) {
            sched_change_priority(thread, THREAD_PRIORITY_MAIN);
            printf("Changed priority of thread %d\n", pids[i]);
        } else {
            printf("Error getting thread %d\n", pids[i]);
        }
    }

    return 0;
}
