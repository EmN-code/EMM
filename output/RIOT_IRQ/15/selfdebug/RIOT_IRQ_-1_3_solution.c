#include <irq_arch.h>
#include <stdatomic.h>
#include <stdio.h>
#include <xtimer.h>

// Global atomic variable to simulate interrupt status
atomic_bool irq_status = false;

// Function to disable interrupts and set status
void disable_interrupts(void) {
    irq_disable();
    atomic_store(&irq_status, true);
}

// Function to restore interrupts and reset status
void restore_interrupts(void) {
    irq_enable();
    atomic_store(&irq_status, false);
}

// Simulation function that triggers the interrupt handling
void simulate_interrupt_handling(void) {
    if (atomic_load(&irq_status)) {
        printf("Interrupts are currently disabled.\n");
    } else {
        printf("Interrupts are currently enabled.\n");
    }
}

int main(void) {
    printf("Starting interrupt handling simulation.\n");
    
    printf("Disabling interrupts...\n");
    disable_interrupts();
    simulate_interrupt_handling();

    printf("Restoring interrupts...\n");
    restore_interrupts();
    simulate_interrupt_handling();

    printf("Interrupt handling simulation completed.\n");
    return 0;
}