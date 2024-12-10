#include <irq_arch.h>

int main() {
    // Step 1: Disable interrupts
    unsigned state = irq_disable();
    
    // Step 2: Critical section
    // Perform operations that must not be interrupted
    
    // Example: Atomic variable simulation
    volatile int atomic_var = 0;
    atomic_var++; // Simulated atomic operation

    // Step 3: Restore interrupts
    irq_restore(state);

    // Check if operations were successful and print status
    if (atomic_var == 1) {
        printf("Atomic operation successful. Interrupts handled correctly.\n");
    } else {
        printf("Atomic operation failed. Issue with interrupt handling.\n");
    }

    return 0;
}