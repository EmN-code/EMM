#include <thread_arch.h>
#include <thread_config.h>
#include <net/protnum.h>
#include <hal/gpio_types.h>

// Define thread function 1
void *thread_function1(void *arg) {
    while (1) {
        printf("Thread 1 is running at priority %d\n", thread_get_priority(thread_getpid()));
        // Simulate some work
        thread_sleep();
    }
    return NULL;
}

// Define thread function 2
void *thread_function2(void *arg) {
    while (1) {
        printf("Thread 2 is running at priority %d\n", thread_get_priority(thread_getpid()));
        // Simulate some work
        thread_sleep();
    }
    return NULL;
}

int main(void) {
    // Define thread and stack for thread 1
    char stack1[THREAD_STACKSIZE_MAIN];
    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1), 5, THREAD_CREATE_STACKTEST, thread_function1, NULL, "thread1");
    
    // Define thread and stack for thread 2
    char stack2[THREAD_STACKSIZE_MAIN];
    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2), 7, THREAD_CREATE_STACKTEST, thread_function2, NULL, "thread2");

    // Simulate some initial work
    thread_sleep();

    // Change the priority of the threads
    printf("Changing priorities...\n");
    sched_set_priority(pid1, 8);
    sched_set_priority(pid2, 4);

    // Print the new priorities
    printf("Thread 1 new priority: %d\n", thread_get_priority(pid1));
    printf("Thread 2 new priority: %d\n", thread_get_priority(pid2));
    
    // Make sure main thread does not terminate
    while (1) {
        thread_sleep();
    }

    return 0;
}