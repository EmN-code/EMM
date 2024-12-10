#include "irq_arch.h"
#include <stdio.h>
#include <stdint.h>

volatile uint32_t irq_disabled_state;

// Simulated atomic variable
volatile int atomic_counter = 0;

// Simulate irq disable function
unsigned irq_disable(void) {
    // This would actually disable interrupts on actual hardware
    // For simulation, we simply return a mock state
    return 1;
}

// Simulate irq restore function
void irq_restore(unsigned state) {
    // This would actually restore interrupts on actual hardware
    // Check the state to see if restored correctly
    if (state == 1) {
        printf("irq restored successfully\n");
    } else {
        printf("irq restoration failed\n");
    }
}

int main() {
    printf("Starting the interrupt handling test...\n");
    
    // Disable interrupts and store the state
    irq_disabled_state = irq_disable();
    
    // Modify atomic variable
    atomic_counter++;
    printf("Atomic counter after disable: %d\n", atomic_counter);
    
    // Restore interrupts
    irq_restore(irq_disabled_state);
    
    // Verify if the atomic operation was successful
    printf("Atomic counter final value: %d\n", atomic_counter);

    return 0;
}