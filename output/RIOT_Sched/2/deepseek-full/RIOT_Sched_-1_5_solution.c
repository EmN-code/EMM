
#include "kernel_defines.h"
#include "msg.h"
#include "thread.h"

#define STACKSIZE 512
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

char stack1[STACKSIZE];
char stack2[STACKSIZE];

void *thread_func1(void *arg) {
    (void)arg;
    while (1) {
        // Thread 1 logic
    }
    return NULL;
}

void *thread_func2(void *arg) {
    (void)arg;
    while (1) {
        // Thread 2 logic
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;

    // Create the first thread
    pid1 = thread_create(stack1, STACKSIZE, PRIORITY_HIGH, 0, thread_func1, NULL, "thread1");
    if (pid1 <= 0) {
        // Handle error
        return 1;
    }

    // Create the second thread
    pid2 = thread_create(stack2, STACKSIZE, PRIORITY_LOW, 0, thread_func2, NULL, "thread2");
    if (pid2 <= 0) {
        // Handle error
        return 1;
    }

    // Change the priority of the first thread to low
    sched_change_priority(pid1, PRIORITY_LOW);

    // Change the priority of the second thread to high
    sched_change_priority(pid2, PRIORITY_HIGH);

    return 0;
}
