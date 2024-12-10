#include <stdio.h>
#include <inttypes.h>
#include <irq.h>
#include <atomic_ops.h>

// Global atomic variable to simulate interrupt status
static atomic_t irq_disabled = ATOMIC_INIT(0);

// Interrupt handler function
void my_interrupt_handler(void) {
    printf("Interrupt handled\n");
}

int main(void) {
    printf("Program starting...\n");
    
    // Disable interrupts
    irq_disable();
    printf("Interrupts disabled\n");
    atomic_set(&irq_disabled, 1); // Set atomic variable indicating interrupts are disabled

    // Simulate calling the interrupt handler while interrupts are disabled
    if (atomic_get(&irq_disabled)) {
        printf("Interrupt cannot be handled yet as interrupts are disabled\n");
    }

    // Enable interrupts
    irq_restore(0);
    printf("Interrupts restored\n");
    atomic_set(&irq_disabled, 0); // Reset atomic variable indicating interrupts are enabled

    // Now call the interrupt handler
    my_interrupt_handler();

    return 0;
}