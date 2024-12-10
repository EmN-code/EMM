#include <stdio.h>
#include <irq_arch.h>

volatile unsigned int atomic_var = 0; // Atomic variable to demonstrate atomic operations

// Function to simulate an interrupt service routine (ISR)
void interrupt_service_routine(void) {
    // Simulate some processing done by the ISR
    atomic_var++; // Increment atomic variable as part of the ISR
    printf("ISR executed. Atomic var incremented to: %u\n", atomic_var);
}

int main(void) {
    printf("Starting program with atomic var value: %u\n", atomic_var);
    
    // Disable interrupts and verify
    unsigned state = irq_disable(); // Disable interrupts
    printf("Interrupts disabled. State: %u\n", state);

    // Assume some critical section code here
    
    // Manually call ISR to simulate interrupt handling
    interrupt_service_routine();

    // Restore interrupts from the previous state and verify
    irq_restore(state); // Restore interrupts
    printf("Interrupts restored to state: %u\n", state);

    // Verify that atomic operations worked correctly
    printf("Final atomic var value: %u\n", atomic_var);
    
    return 0;
}