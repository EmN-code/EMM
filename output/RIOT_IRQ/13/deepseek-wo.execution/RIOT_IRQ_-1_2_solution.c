
#include <stdio.h>
#include <irq.h>
#include <stdatomic.h>
#include <event/thread.h>
#include <thread.h>

// Define an atomic variable
atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Interrupt handler function
void irq_handler(void)
{
    // Increment the atomic variable inside the interrupt handler
    atomic_fetch_add(&atomic_var, 1);
}

int main(void)
{
    // Store the current interrupt state
    unsigned int state = irq_disable();

    // Set the interrupt handler
    irq_set_handler(0, irq_handler);

    // Restore the interrupt state
    irq_restore(state);

    // Enable interrupts
    irq_enable();

    // Main loop to check the atomic variable
    while (1) {
        // Load the current value of the atomic variable
        int current_value = atomic_load(&atomic_var);

        // Print the current value
        printf("Atomic variable value: %d\n", current_value);

        // Sleep for a while to avoid busy-waiting
        thread_sleep();
    }

    return 0;
}
