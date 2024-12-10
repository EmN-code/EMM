#include <stdio.h>
#include <stdlib.h>
#include <thread.h>

#define THREAD_STACKSIZE_DEFAULT 1024
#define THREAD_PRIORITY_MAIN 5

char stack1[THREAD_STACKSIZE_DEFAULT];
char stack2[THREAD_STACKSIZE_DEFAULT];
kernel_pid_t pid1, pid2;

void *thread_one(void *arg) {
    (void)arg;
    printf("Thread 1 running\n");
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    printf("Thread 2 running\n");
    return NULL;
}

int main(void) {
    pid1 = thread_create(stack1, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN, 0, thread_one, NULL, "thread_one");
    pid2 = thread_create(stack2, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN, 0, thread_two, NULL, "thread_two");
    
    if (pid1 >= 0) {
        sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN - 1);
    }
    if (pid2 >= 0) {
        sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN - 2);
    }
    
    while(1) {}

    return 0;
}