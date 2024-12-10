#include "thread_config.h"
#include "cpu_conf.h"
#include "stdio.h"
#include "kernel.h"

#define THREAD_STACKSIZE 1024
#define THREAD_PRIORITY_MAIN 5

char stack1[THREAD_STACKSIZE];
char stack2[THREAD_STACKSIZE];

kernel_pid_t thread1;
kernel_pid_t thread2;

void *thread_handler(void *arg) {
    (void) arg;
    printf("Thread %d running\n", thread_getpid());
    return NULL;
}

int main(void) {
    printf("Main started\n");

    thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 
                            THREAD_CREATE_STACKTEST, thread_handler, NULL, "thread1");
    thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1, 
                            THREAD_CREATE_STACKTEST, thread_handler, NULL, "thread2");

    sched_change_priority(thread_get(thread1), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(thread2), THREAD_PRIORITY_MAIN - 1);

    return 0;
}