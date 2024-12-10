#include <stdio.h>
#include <thread.h>
#include <kernel_types.h>

#define EXAMPLE_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

// Define two different priorities for our threads
#define THREAD_PRIORITY_THREAD_ONE (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_THREAD_TWO (THREAD_PRIORITY_MAIN - 2)

char stackOne[EXAMPLE_THREAD_STACKSIZE];
char stackTwo[EXAMPLE_THREAD_STACKSIZE];

void *threadOneHandler(void *arg) {
    (void)arg;
    printf("Thread One: Running at priority %d\n", THREAD_PRIORITY_THREAD_ONE);

    // Simulate some work with a sleep
    thread_sleep(2);

    printf("Thread One: Completed\n");
    return NULL;
}

void *threadTwoHandler(void *arg) {
    (void)arg;
    printf("Thread Two: Running at priority %d\n", THREAD_PRIORITY_THREAD_TWO);

    // Simulate some work with a sleep
    thread_sleep(3);

    printf("Thread Two: Completed\n");
    return NULL;
}

int main(void) {
    kernel_pid_t pid1 = thread_create(stackOne, sizeof(stackOne), THREAD_PRIORITY_THREAD_ONE,
                                       THREAD_CREATE_STACKTEST, threadOneHandler, NULL, "thread_one");
    if (pid1 == KERNEL_PID_UNDEF) {
        printf("Error: Unable to create thread one\n");
        return -1;
    }

    kernel_pid_t pid2 = thread_create(stackTwo, sizeof(stackTwo), THREAD_PRIORITY_THREAD_TWO,
                                       THREAD_CREATE_STACKTEST, threadTwoHandler, NULL, "thread_two");
    if (pid2 == KERNEL_PID_UNDEF) {
        printf("Error: Unable to create thread two\n");
        return -1;
    }

    // Demonstrate changing the priority of thread one to the same as thread two
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_THREAD_TWO);
    printf("Main: Changed priority of thread one to %d\n", THREAD_PRIORITY_THREAD_TWO);
    return 0;
}