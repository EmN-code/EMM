
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <esp_system.h>
#include <xtimer.h>
#include <thread.h>
#include <mutex.h>
#include <msg.h>

// Define a simple atomic variable
atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Function to be executed by the thread
void thread_func(void *arg) {
    (void)arg;
    printf("Thread started\n");
    // Simulate some work
    xtimer_sleep(2);
    printf("Thread finished\n");
}

int main(void) {
    // Create a stack for the thread
    char stack[THREAD_STACKSIZE_DEFAULT];

    // Create a thread
    kernel_pid_t pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, thread_func, NULL, "worker");

    // Check if thread creation was successful
    if (pid <= 0) {
        printf("Failed to create thread\n");
        return 1;
    }

    // Disable interrupts
    unsigned state = irq_disable();

    // Modify the atomic variable in an atomic way
    atomic_fetch_add(&atomic_var, 1);

    // Restore interrupts
    irq_restore(state);

    // Wait for the thread to finish
    thread_join(pid);

    // Check the value of the atomic variable
    if (atomic_load(&atomic_var) == 1) {
        printf("Interrupts were correctly disabled and restored\n");
    } else {
        printf("Interrupts were not correctly handled\n");
    }

    return 0;
}
