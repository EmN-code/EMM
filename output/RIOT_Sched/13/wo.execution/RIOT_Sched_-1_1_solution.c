#include <thread.h>
#include <stdio.h>
#include <stdlib.h>

#define THREAD_STACKSIZE_MAIN    (KERNEL_CONF_STACKSIZE_MAIN)
#define MAIN_THREAD_PRIORITY     (THREAD_PRIORITY_MAIN - 1)
#define SECOND_THREAD_PRIORITY   (MAIN_THREAD_PRIORITY + 1)

char stack_thread_one[THREAD_STACKSIZE_MAIN];
char stack_thread_two[THREAD_STACKSIZE_MAIN];

void *thread_one(void *arg) {
    puts("Thread one is running.");
    thread_t *current = thread_getpid();
    thread_set_priority(current, SECOND_THREAD_PRIORITY);
    puts("Thread one changed its priority.");
    return NULL;
}

void *thread_two(void *arg) {
    puts("Thread two is running.");
    thread_t *current = thread_getpid();
    thread_set_priority(current, MAIN_THREAD_PRIORITY);
    puts("Thread two changed its priority.");
    return NULL;
}

int main(void) {
    puts("Starting threads on RIOT");
    kernel_pid_t p1 = thread_create(stack_thread_one, sizeof(stack_thread_one), 
                                    MAIN_THREAD_PRIORITY, 0, thread_one, NULL, "thread_one");
    kernel_pid_t p2 = thread_create(stack_thread_two, sizeof(stack_thread_two), 
                                    SECOND_THREAD_PRIORITY, 0, thread_two, NULL, "thread_two");
    
    if (p1 < 0 || p2 < 0) {
        puts("Error: unable to create threads");
        return 1;
    }
    
    // Let's join the threads if possible
    // RIOT may not support thread joining, hence just simulate some wait
    while(1) {
        continue;
    }
    return 0;
}