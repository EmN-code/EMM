#include <irq_arch.h>
#include <stdbool.h>
#include <stdio.h>

volatile bool irq_status = false;

void my_interrupt_handler(void) {
    // Simulate an action during an interrupt
    printf("Interrupt occurred!\n");
}

int main() {
    // Disable interrupts
    unsigned state = irq_disable();
    printf("Interrupts disabled\n");

    // Simulate setting an IRQ status atomically
    irq_status = true;
    printf("IRQ status set to true\n");

    // Restore interrupts
    irq_restore(state);
    printf("Interrupts restored\n");

    // Verify the irq status
    if (irq_status) {
        // This would typically be triggered by a real interrupt
        my_interrupt_handler();
    }

    return 0;
}