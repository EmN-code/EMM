
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>
#include <atomic_utils.h> // Include the correct header for atomic operations

volatile atomic_t atomic_var = ATOMIC_INIT(0); // Initialize an atomic variable

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_store(&atomic_var, 1); // Set atomic variable to 1
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Disable interrupts
    unsigned state = irq_disable();

    // Perform some operations while interrupts are disabled
    printf("Interrupts are disabled. Performing operations...\n");
    xtimer_sleep(2); // Simulate some work

    // Verify that the atomic variable is still 0
    if (atomic_load(&atomic_var) == 0) {
        printf("Atomic variable is still 0, as expected.\n");
    } else {
        printf("Error: Atomic variable changed while interrupts were disabled.\n");
    }

    // Restore interrupts to their previous state
    irq_restore(state);

    // Enable interrupts and trigger an interrupt
    printf("Interrupts are restored. Triggering an interrupt...\n");
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1, 0, interrupt_handler, NULL, "interrupt_handler");

    // Wait for the interrupt to be handled
    xtimer_sleep(2);

    // Verify that the atomic variable was set to 1 by the interrupt handler
    if (atomic_load(&atomic_var) == 1) {
        printf("Atomic variable set to 1 by interrupt handler, as expected.\n");
    } else {
        printf("Error: Atomic variable was not set by interrupt handler.\n");
    }

    printf("Interrupt handling test completed.\n");
    return 0;
}
