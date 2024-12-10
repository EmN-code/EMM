#include <thread_arch.h>
#include <thread_config.h>
#include <stdio.h>

void *thread_handler_1(void *arg) {
    printf("Thread 1 running with ID: %d\n", thread_getpid());
    return NULL;
}

void *thread_handler_2(void *arg) {
    printf("Thread 2 running with ID: %d\n", thread_getpid());
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;
    
    /* Start thread 1 */
    pid1 = thread_create(NULL, THREAD_STACKSIZE_MAIN, THREAD_PRIORITY_MAIN, 0, thread_handler_1, NULL, "thread_1");
    printf("Started Thread 1 with PID: %d\n", pid1);
    
    /* Start thread 2 */
    pid2 = thread_create(NULL, THREAD_STACKSIZE_MAIN, THREAD_PRIORITY_MAIN - 1, 0, thread_handler_2, NULL, "thread_2");
    printf("Started Thread 2 with PID: %d\n", pid2);
    
    /* Change priority of thread 1 to lower than main */
    sched_change_priority(pid1, THREAD_PRIORITY_MAIN + 1);
    printf("Changed priority of Thread 1\n");
    
    /* Yield to allow the threads to execute */
    thread_yield();

    return 0;
}