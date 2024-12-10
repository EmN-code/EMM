#include "xtimer.h"
#include "stdlib.h"
#include "stddef.h"
#include "mutex.h"
#include "msg.h"

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN 5

char thread_one_stack[THREAD_STACKSIZE_MAIN];
char thread_two_stack[THREAD_STACKSIZE_MAIN];
kernel_pid_t thread_one_pid;
kernel_pid_t thread_two_pid;

void *thread_func_one(void *arg) {
    while(1) {
        puts("Thread One running...");
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread_func_two(void *arg) {
    while(1) {
        puts("Thread Two running...");
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    thread_one_pid = thread_create(thread_one_stack, sizeof(thread_one_stack),
                                   THREAD_PRIORITY_MAIN, 0,
                                   thread_func_one, NULL, "thread_one");
                                    
    thread_two_pid = thread_create(thread_two_stack, sizeof(thread_two_stack),
                                   THREAD_PRIORITY_MAIN - 1, 0,
                                   thread_func_two, NULL, "thread_two");

    xtimer_sleep(5); // Run for some time at initial priority

    // Increase the priority of thread two
    printf("Changing priority of thread two\n");
    sched_change_priority(thread_get(thread_two_pid), THREAD_PRIORITY_MAIN - 2);

    xtimer_sleep(10); // Allow time for observation

    return 0;
}