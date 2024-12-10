#include "thread_arch.h"
#include "thread_config.h"
#include "thread.h"
#include "event/thread.h"
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 1)
#define THREAD_SLEEP_TIME       (1000000U) 

char stack1[THREAD_STACKSIZE_MAIN];
char stack2[THREAD_STACKSIZE_MAIN];

void *thread_func(void *arg) {
    while (1) {
        printf("Thread %d running with priority %d\n", (int)arg, thread_get_priority(NULL));
        xtimer_usleep(THREAD_SLEEP_TIME); 
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;

    puts("Creating threads...\n");

    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST, thread_func, (void *)1, "thread 1");
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, thread_func, (void *)2, "thread 2");

    printf("Threads created with PIDs %d and %d\n", pid1, pid2);

    xtimer_sleep(5);  

    printf("Changing priority of thread 1\n");
    thread_set_priority(pid1, THREAD_PRIORITY_MAIN - 3);
    
    while (1) {
        xtimer_usleep(THREAD_SLEEP_TIME); 
        printf("Main thread running.\n");
    }
    return 0;
}