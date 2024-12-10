#include <stdbool.h>
#include <stdatomic.h>
#include "irq_arch.h"

// Function to simulate application logic for interrupt handling
void application_logic(void) {
    // Define an atomic variable
    atomic_bool interrupt_status = false;
    
    // Disable interrupts and perform atomic operation
    irq_disable();
    // Toggle atomic variable value
    atomic_store(&interrupt_status, true);
    // Restore interrupts after operation
    irq_restore();
    
    // Output result to ensure atomic operation was successful
    if (atomic_load(&interrupt_status)) {
        printf("Interrupt was disabled and restored successfully.\n");
    }
    else {
        printf("Failed to disable/restore interrupt correctly.\n");
    }
}

int main(void) {
    // Main program execution
    printf("Starting interrupt handling verification on ESP32.\n");
    application_logic();
    return 0;
}