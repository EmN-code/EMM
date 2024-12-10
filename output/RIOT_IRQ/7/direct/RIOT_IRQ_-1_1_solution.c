#include <irq.h>
#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <stdatomic.h>

// Define stack size
#define STACK_SIZE_THREAD (THREAD_STACKSIZE_MAIN)

// Define a global atomic variable
atomic_int interrupt_enabled;

// Thread stack and pid
char stack_thread[STACK_SIZE_THREAD];
kernel_pid_t thread_pid;

// Simulated interrupt handler function
void interrupt_handler(void) {
    irq_disable();
    printf("Interrupts disabled in handler\n");

    // Verify that interrupts were disabled
    atomic_store(&interrupt_enabled, 0);

    // Simulate heavy processing
    xtimer_sleep(1);

    irq_enable();
    printf("Interrupts restored in handler\n");

    // Verify that interrupts were restored
    atomic_store(&interrupt_enabled, 1);
}

// Thread function that calls the interrupt handler
void *thread_func(void *arg) {
    (void)arg;
    while (1) {
        interrupt_handler();
        xtimer_sleep(3);
    }
    return NULL;
}

int main(void) {
    printf("RIOT interrupt handling example\n");

    // Initialize the atomic variable
    atomic_init(&interrupt_enabled, 1);

    // Create a thread that will simulate interrupts
    thread_pid = thread_create(stack_thread, sizeof(stack_thread), THREAD_PRIORITY_MAIN - 1,
                               THREAD_CREATE_STACKTEST, thread_func, NULL, "interrupt_thread");

    // Main thread loop
    while (1) {
        xtimer_sleep(1);

        // Check if interrupts are enabled using atomic variable
        if (atomic_load(&interrupt_enabled)) {
            printf("Main: Interrupts are enabled\n");
        } else {
            printf("Main: Interrupts are disabled\n");
        }
    }

    return 0;
}