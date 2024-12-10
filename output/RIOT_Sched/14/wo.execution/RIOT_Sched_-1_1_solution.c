#include "thread_config.h"
#include "thread_arch.h"
#include <stdio.h>

#define STACKSIZE_THREAD (THREAD_STACKSIZE_DEFAULT)

char stack_thread1[STACKSIZE_THREAD];
char stack_thread2[STACKSIZE_THREAD];

void *thread_handler1(void *arg){
    while (1) {
        printf("Thread 1 running.\n");
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread_handler2(void *arg){
    while (1) {
        printf("Thread 2 running.\n");
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void){
    kernel_pid_t thread1_pid = thread_create(stack_thread1, sizeof(stack_thread1), 
                                               THREAD_PRIORITY_MAIN - 1, 0, 
                                               thread_handler1, NULL, "thread1");
    printf("Thread 1 PID: %d\n", thread1_pid);

    kernel_pid_t thread2_pid = thread_create(stack_thread2, sizeof(stack_thread2), 
                                               THREAD_PRIORITY_MAIN - 1, 0, 
                                               thread_handler2, NULL, "thread2");
    printf("Thread 2 PID: %d\n", thread2_pid);

    // Change priority of thread1 to be higher
    thread_set_priority(thread1_pid, THREAD_PRIORITY_MAIN - 2);
    printf("Changed priority of Thread 1\n");

    // Change priority of thread2 to be lower
    thread_set_priority(thread2_pid, THREAD_PRIORITY_MAIN - 3);
    printf("Changed priority of Thread 2\n");

    while (1){
        thread_yield();
    }
    return 0;
}