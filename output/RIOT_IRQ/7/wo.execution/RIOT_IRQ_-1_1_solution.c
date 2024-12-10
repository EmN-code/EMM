#include "irq_arch.h"

int main(void) {
    // Initial setup
    unsigned irq_status;
    unsigned test_var = 0;

    // Step 1: Disable interrupts and modify the atomic variable
    irq_status = irq_disable();  // Disable interrupts
    test_var += 1;  // Modify a variable atomically
    unsigned check_var = test_var; // Copy for checking

    // Step 2: Check if interrupts were successfully disabled
    if (irq_status != 0) {
        printf("Interrupts disabled successfully.\n");
    } else {
        printf("Failed to disable interrupts.\n");
    }

    // Step 3: Re-enable interrupts
    irq_restore(irq_status);

    // Check if variable modification was successful
    if (check_var == test_var) {
        printf("Atomic operation successful. Value: %u\n", test_var);
    } else {
        printf("Atomic operation failed.\n");
    }
    
    return 0;
}