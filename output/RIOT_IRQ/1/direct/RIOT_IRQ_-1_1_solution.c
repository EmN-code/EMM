// Description of the code solution
// This program demonstrates interrupt handling in RIOT OS on an ESP32 using atomic variables. 
// We use the irq_disable() and irq_restore() functions to manipulate interrupts.
// Execution will show if disabling and restoring interrupts works correctly via changes in atomic variable.

#include <stdio.h>
#include <stdint.h>
#include <irq.h>
#include <inttypes.h>
#include <atomic.h>
#include <xtimer.h>

// Global atomic variable
atomic_t my_atomic_var = ATOMIC_INIT(0);

void trigger_interrupt(void) {
    // Simulate critical section by disabling interrupts
    irq_disable();

    // Increment the atomic variable
    atomic_set(&my_atomic_var, atomic_get(&my_atomic_var) + 1);

    // Simulate end of critical section by restoring interrupts
    irq_restore();
}

void *thread_function(void *arg) {
    (void)arg;
    while(1) {
        trigger_interrupt();
        // Print the atomic variable value
        printf("Atomic var: %" PRIu32 "\n", atomic_get(&my_atomic_var));
        // Sleep for some time to observe the changes
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    puts("Starting the interrupt handling with atomic verification example\n");

    // Create a basic thread to simulate interrupts
    thread_create(0, 0, 0, THREAD_PRIORITY_MAIN - 1, thread_function, NULL, "interrupt_thread");

    return 0;
}