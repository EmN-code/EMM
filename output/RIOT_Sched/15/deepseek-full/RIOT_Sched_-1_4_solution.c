
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <thread.h>
#include <stdatomic.h>

#define STACKSIZE 1024

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        sleep(1);
    }
    return NULL;
}

void *thread2_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
        sleep(1);
    }
    return NULL;
}

int main(void) {
    char stack1[STACKSIZE], stack2[STACKSIZE];

    // Create the first thread
    thread1_pid = thread_create(stack1, STACKSIZE, THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");

    // Create the second thread
    thread2_pid = thread_create(stack2, STACKSIZE, THREAD_PRIORITY_MAIN - 2,
                                THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    // Wait for 5 seconds
    sleep(5);

    // Change the priority of the first thread
    thread_change_priority(thread1_pid, THREAD_PRIORITY_MAIN - 2);

    // Change the priority of the second thread
    thread_change_priority(thread2_pid, THREAD_PRIORITY_MAIN - 1);

    // Main thread continues to run
    while (1) {
        printf("Main thread running\n");
        sleep(1);
    }

    return 0;
}
