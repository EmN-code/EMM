
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <time_units.h>
#include <timex.h>
#include <mbox.h>
#include <msg.h>
#include <sched.h>
#include <thread.h>

#define STACKSIZE 1024
#define NUM_THREADS 2

static char stack[NUM_THREADS][STACKSIZE];
static kernel_pid_t pids[NUM_THREADS];

void thread_func(void *arg) {
    int id = (int)arg;
    printf("Thread %d started\n", id);
    while (1) {
        printf("Thread %d running\n", id);
        sleep(1);
    }
}

int main(void) {
    puts("RIOT multi-threading example");

    for (int i = 0; i < NUM_THREADS; i++) {
        pids[i] = thread_create(stack[i], STACKSIZE, THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST, thread_func, (void *)i, "thread");
    }

    // Change priority of the first thread
    sched_change_priority(thread_get(pids[0]), THREAD_PRIORITY_MAIN);

    // Change priority of the second thread
    sched_change_priority(thread_get(pids[1]), THREAD_PRIORITY_MAIN - 2);

    return 0;
}
