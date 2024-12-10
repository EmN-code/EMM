// Solution to fix implicit function declaration and redefinition errors

#include <xtimer.h>         // Necessary for xtimer_sleep
#include <thread.h>         // Necessary for thread_create and managing thread priorities
#include <sched.h>          // Necessary for threading and scheduler functions
#include <stdio.h>          // Necessary for using printf
#include <inttypes.h>       // Necessary for fixed width integer types

#define MAIN_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define STACKSIZE_MAIN THREAD_STACKSIZE_DEFAULT
#define THREAD_CREATE_STACKTEST (8)  // Define if not previously defined

char stack_thread_one[THREAD_STACKSIZE_MAIN];
char stack_thread_two[THREAD_STACKSIZE_MAIN];

void *thread_func_one(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 running with priority %d\n", thread_get_priority(thread_getpid()));
        thread_yield();
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_func_two(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 running with priority %d\n", thread_get_priority(thread_getpid()));
        thread_yield();
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread_one_pid = thread_create(stack_thread_one, sizeof(stack_thread_one),
                                                MAIN_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                                                thread_func_one, NULL, "thread_one");

    kernel_pid_t thread_two_pid = thread_create(stack_thread_two, sizeof(stack_thread_two),
                                                MAIN_THREAD_PRIORITY + 1, THREAD_CREATE_STACKTEST,
                                                thread_func_two, NULL, "thread_two");

    while(1) {
        // Change priority
        sched_change_priority(thread_one_pid, MAIN_THREAD_PRIORITY + 2);
        sched_change_priority(thread_two_pid, MAIN_THREAD_PRIORITY);
        xtimer_sleep(5);

        // Restore priority
        sched_change_priority(thread_one_pid, MAIN_THREAD_PRIORITY);
        sched_change_priority(thread_two_pid, MAIN_THREAD_PRIORITY + 1);
        xtimer_sleep(5);
    }
    return 0;
}