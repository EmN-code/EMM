#include <irq_arch.h>
#include <stdio.h>
#include <stdatomic.h>

atomic_bool interrupt_status;

void irq_example(void) {
    unsigned state;

    // Disable interrupts and get the initial state
    state = irq_disable();
    printf("Interrupts disabled\n");

    // Manipulate shared data
    interrupt_status = false;
    printf("Interrupt status changed to false\n");

    // Restore interrupts to previous state
    irq_restore(state);
    printf("Interrupts restored\n");

    // Verify if the interrupts were enabled again
    if (irq_is_enabled()) {
        printf("Interrupts are enabled.\n");
    } else {
        printf("Interrupts are still disabled.\n");
    }
}

int main(void) {
    // Initial status of the interrupt
    interrupt_status = true;
    printf("Initial interrupt status: %d\n", interrupt_status);

    // Perform IRQ example function
    irq_example();

    // Check status again
    printf("Final interrupt status: %d\n", interrupt_status);
    
    return 0;
}
