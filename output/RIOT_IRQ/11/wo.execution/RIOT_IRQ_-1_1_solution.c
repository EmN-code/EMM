#include <irq_arch.h>
#include <stdio.h>

volatile unsigned irq_state; // global variable for interrupt state
volatile int atomic_variable; // atomic variable to demonstrate use

void simulate_isr(void) {
    // Simulating an Interrupt Service Routine
    printf("Interrupt Service Routine triggered!\n");
    atomic_variable++;
}

void disable_and_enable_irqs(void) {
    irq_state = irq_disable();
    printf("IRQs disabled. Current atomic_variable: %d\n", atomic_variable);

    // Simulate some critical operations where interrupts should be disabled
    for (int i = 0; i < 5; ++i) {
        atomic_variable += i;
    }
    
    irq_restore(irq_state);
    printf("IRQs restored. Current atomic_variable: %d\n", atomic_variable);
}

int main(void) {
    atomic_variable = 0;
    printf("Initial atomic_variable: %d\n", atomic_variable);

    disable_and_enable_irqs();

    // Simulate an interrupt
    simulate_isr();
    printf("After ISR, atomic_variable: %d\n", atomic_variable);

    return 0;
}