#include "lwip/pbuf.h"
#include "irq_arch.h"

#include <stdio.h>

atomic_bool irq_was_enabled;

void test_irq_handling(void) {
    // Disable interrupts and check if it was enabled before
    irq_was_enabled = irq_disable();

    // Simulate some critical section processing
    printf("Interrupts temporarily disabled\n");

    // Restore the interrupt state based on previous state
    if (irq_was_enabled) {
        irq_restore(irq_was_enabled);
    }

    printf("Interrupts state restored\n");
}

int main(void) {
    printf("Starting IRQ handling test\n");

    // Test the irq handling function
    test_irq_handling();

    return 0;
}
