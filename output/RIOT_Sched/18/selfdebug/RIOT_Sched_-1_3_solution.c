#include <thread.h>
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)  // Ensures it's correctly defined once
#define THREAD_PRIORITY_MAIN     (10U)  // Correct priority value

static char stack_one[THREAD_STACKSIZE_MAIN];
static char stack_two[THREAD_STACKSIZE_MAIN];

static void *thread_one_handler(void *arg) {
    (void) arg;  // Unused
    puts("Thread One Running");
    return NULL;
}

static void *thread_two_handler(void *arg) {
    (void) arg;  // Unused
    puts("Thread Two Running");
    return NULL;
}

int main(void) {
    kernel_pid_t pid_one, pid_two;

    // Creating Thread One
    pid_one = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN - 1,
                            THREAD_CREATE_STACKTEST, thread_one_handler, NULL, "thread_one");

    // Creating Thread Two
    pid_two = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_MAIN - 2,
                            THREAD_CREATE_STACKTEST, thread_two_handler, NULL, "thread_two");

    // Change priorities at runtime
    sched_change_priority(thread_get(pid_one), THREAD_PRIORITY_MAIN - 3);
    sched_change_priority(thread_get(pid_two), THREAD_PRIORITY_MAIN - 4);

    return 0;
}