#include "thread.h"
#include "stdio.h"

#define THREAD_STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN (1)

char thread_stack_one[THREAD_STACK_SIZE];
char thread_stack_two[THREAD_STACK_SIZE];

void *thread_handler_one(void *arg) {
    printf("Thread 1 is running\n");
    return NULL;
}

void *thread_handler_two(void *arg) {
    printf("Thread 2 is running\n");
    return NULL;
}

int main(void) {
    // Create the first thread
    kernel_pid_t thread_one = thread_create(thread_stack_one, sizeof(thread_stack_one),
                                            THREAD_PRIORITY_MAIN, 0,
                                            thread_handler_one, NULL, "Thread 1");
    // Create the second thread
    kernel_pid_t thread_two = thread_create(thread_stack_two, sizeof(thread_stack_two),
                                            THREAD_PRIORITY_MAIN + 1, 0,
                                            thread_handler_two, NULL, "Thread 2");

    // Change the priority of the first thread
    sched_change_priority(thread_get(thread_one), THREAD_PRIORITY_MAIN + 2);
    // Change the priority of the second thread
    sched_change_priority(thread_get(thread_two), THREAD_PRIORITY_MAIN + 3);

    printf("Main thread: Threads created and priorities adjusted\n");

    // Let the main thread wait indefinitely
    thread_sleep();

    return 0;
}