#include <thread_config.h>
#include <stdio.h>

void *thread_func1(void *arg) {
    while (1) {
        // Task for thread 1
        printf("Running thread 1\n");
    }
    return NULL;
}

void *thread_func2(void *arg) {
    while (1) {
        // Task for thread 2
        printf("Running thread 2\n");
    }
    return NULL;
}

int main(void) {
    printf("Starting threads\n");
    
    // Define and create two threads
    kernel_pid_t thread1 = thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN+1, 0, thread_func1, NULL, "thread1");
    kernel_pid_t thread2 = thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN+2, 0, thread_func2, NULL, "thread2");
    
    // Change the priority of thread 2
    sched_change_priority(thread_get(thread2), THREAD_PRIORITY_MAIN-1);
    
    while(1) {
        // Main loop doing nothing
    }

    return 0;  
}