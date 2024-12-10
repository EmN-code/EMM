#include "irq_arch.h"
#include <stdio.h>
#include "xtimer.h"

volatile unsigned irq_disable_count = 0;
volatile unsigned irq_restore_count = 0;
volatile bool irq_status_before = false;
volatile bool irq_status_after = false;

void test_irq_behavior(void) {
    // Check the status of IRQ before disabling
    irq_status_before = irq_is_enabled();
    
    // Disable IRQ
    unsigned state = irq_disable();
    irq_disable_count++;
    
    // Perform operations that require IRQs to be disabled
    xtimer_usleep(500000);  // Sleep for 500ms

    // Restore IRQ
    irq_restore(state);
    irq_restore_count++;
    
    // Check the status of IRQ after restoring
    irq_status_after = irq_is_enabled();
}

int main(void) {
    printf("Starting Interrupt Behavior Test\n");
    test_irq_behavior();
    printf("IRQ disable count: %u\n", irq_disable_count);
    printf("IRQ restore count: %u\n", irq_restore_count);
    printf("IRQ status before: %s\n", irq_status_before ? "enabled" : "disabled");
    printf("IRQ status after: %s\n", irq_status_after ? "enabled" : "disabled");
    return 0;
}