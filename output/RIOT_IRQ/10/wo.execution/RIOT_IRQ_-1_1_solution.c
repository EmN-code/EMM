#include <stdio.h>
#include <irq_arch.h>  
#include <inttypes.h> 
#include <stdatomic.h>

volatile atomic_bool irq_state;

void simulate_interrupt_handling(void) {
    // Disable interrupts and store the current state
    unsigned state = irq_disable();
    
    // Setting atomic variable to verify the interrupts state
    atomic_store(&irq_state, state);
    
    printf("Interrupts have been disabled. Current IRQ State: %u\n", state);

    // Simulate critical section
    // Perform operations that should not be interrupted

    // Restore the previous interrupt state
    irq_restore(state);
    atomic_store(&irq_state, state);
    
    printf("Interrupts have been restored. IRQ State: %u\n", state);
}

int main(void) {
    // Initialize atomic variable
    atomic_init(&irq_state, false);

    // Simulate the interrupt handling process
    simulate_interrupt_handling();
    
    return 0;
}