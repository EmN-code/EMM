#include "thread_arch.h"
#include "thread_config.h"
#include "stdio.h"

#define STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define STACKSIZE_SECONDARY (THREAD_STACKSIZE_DEFAULT)

// Thread function
void *thread1_function(void *arg) {
    (void)arg;
    printf("Thread 1 is running\n");
    // Perform some tasks here
    return NULL;
}

void *thread2_function(void *arg) {
    (void)arg;
    printf("Thread 2 is running\n");
    // Perform some tasks here
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;
    char thread_stack1[STACKSIZE_MAIN];
    char thread_stack2[STACKSIZE_SECONDARY];

    // Starting Thread 1
    pid1 = thread_create(thread_stack1, sizeof(thread_stack1), 
                         THREAD_PRIORITY_MAIN, 0, 
                         thread1_function, NULL, "thread1");
    if (pid1 <= KERNEL_PID_UNDEF) {
        printf("Error: unable to create thread 1\n");
        return 1;
    }

    // Starting Thread 2
    pid2 = thread_create(thread_stack2, sizeof(thread_stack2), 
                         THREAD_PRIORITY_MAIN + 1, 0, 
                         thread2_function, NULL, "thread2");
    if (pid2 <= KERNEL_PID_UNDEF) {
        printf("Error: unable to create thread 2\n");
        return 1;
    }

    // Change priority of Thread 1 at runtime
    int success = sched_set_priority(pid1, THREAD_PRIORITY_MAIN + 2);
    if (success != 0) {
        printf("Unable to change priority of thread 1\n");
        return success;
    }

    printf("Priority of Thread 1 changed\n");

    return 0;
}