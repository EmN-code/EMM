#include <thread_arch.h>
#include <thread_config.h>
#include <stdio.h>
#include <hal/gpio_types.h>

void *thread_function1(void *arg) {
    printf("Thread 1 is running\n");
    // Simulating work
    for (int i = 0; i < 5; i++) {
        printf("Thread 1 working...\n");
    }
    return NULL;
}

void *thread_function2(void *arg) {
    printf("Thread 2 is running\n");
    // Simulating work
    for (int i = 0; i < 5; i++) {
        printf("Thread 2 working...\n");
    }
    return NULL;
}

int main() {
    kernel_pid_t thread1_pid;
    kernel_pid_t thread2_pid;

    char thread1_stack[THREAD_STACKSIZE_MAIN];
    char thread2_stack[THREAD_STACKSIZE_MAIN];

    thread1_pid = thread_create(thread1_stack, sizeof(thread1_stack), THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST, thread_function1, NULL, "Thread 1");

    thread2_pid = thread_create(thread2_stack, sizeof(thread2_stack), THREAD_PRIORITY_MAIN - 2,
                                THREAD_CREATE_STACKTEST, thread_function2, NULL, "Thread 2");

    printf("Threads created\n");

    // Change priority of thread1 to be lower than that of thread2
    sched_change_priority(thread1_pid, THREAD_PRIORITY_MAIN - 3);
    printf("Changed priority of Thread 1\n");

    // Assuming thread join to wait for threads to finish
    pthread_join(thread1_pid, NULL);
    pthread_join(thread2_pid, NULL);

    return 0;
}