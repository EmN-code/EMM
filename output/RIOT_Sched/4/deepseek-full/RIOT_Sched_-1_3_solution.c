
#include <stdio.h>
#include <thread.h>
#include <msg.h>
#include <sched.h>

#define STACK_SIZE 512
#define NUM_THREADS 2

kernel_pid_t threads[NUM_THREADS];
char stacks[NUM_THREADS][STACK_SIZE];

void *thread_func(void *arg) {
    int thread_id = (int)arg;
    printf("Thread %d started with priority %d\n", thread_id, sched_active_thread->priority);
    while (1) {
        printf("Thread %d running\n", thread_id);
        thread_yield();
    }
    return NULL;
}

int main(void) {
    msg_t msg_array[8];
    msg_init_queue(msg_array, 8);

    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i] = thread_create(stacks[i], STACK_SIZE, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, thread_func, (void *)i, "thread");
    }

    // Change priority of the first thread
    sched_change_priority(threads[0], THREAD_PRIORITY_MAIN - 2);
    printf("Changed priority of thread 0 to %d\n", THREAD_PRIORITY_MAIN - 2);

    // Change priority of the second thread
    sched_change_priority(threads[1], THREAD_PRIORITY_MAIN - 3);
    printf("Changed priority of thread 1 to %d\n", THREAD_PRIORITY_MAIN - 3);

    return 0;
}
