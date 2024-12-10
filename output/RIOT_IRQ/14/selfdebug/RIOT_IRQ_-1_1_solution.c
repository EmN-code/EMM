// Description:
// This code demonstrates how to handle interrupts using atomic variables and the `irq` functions in RIOT OS on an ESP32 platform. It verifies the behavior of disabling and restoring interrupts without involving GPIO.

#include <irq_arch.h> // Include first header file
#include <irq_cpu.h> // Include second header file

#include <stdio.h> // Include third header file

#include <stdatomic.h> // Include fourth header file

static atomic_bool interrupt_status; // Declare atomic variable to track interrupt status

void handle_interrupt(void) {
    unsigned was_enabled = irq_disable(); // Disable interrupts
    
    // Simulate critical section of interrupt handling
    printf("Handling interrupt\n");
    atomic_store(&interrupt_status, 1); // Update atomic variable to indicate handling done

    irq_restore(was_enabled); // Restore previous interrupt state
}

void check_interrupt_handling(void) {
    if (atomic_load(&interrupt_status)) { // Verify if interrupt was handled
        printf("Interrupt was handled successfully.\n");
    } else {
        printf("Interrupt was not handled.\n");
    }
}

int main(void) {
    printf("Starting interrupt handling verification on ESP32...\n");
    atomic_store(&interrupt_status, 0); // Initialize interrupt status

    handle_interrupt(); // Simulate an interrupt

    check_interrupt_handling(); // Check if interrupt was handled

    return 0;
}