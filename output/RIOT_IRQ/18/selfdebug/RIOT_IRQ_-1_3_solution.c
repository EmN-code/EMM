#include "thread_arch.h"
#include "thread_flags.h"
#include "irq_arch.h"

#include <stdio.h>
#include "xtimer.h"

volatile unsigned irq_enabled;

void test_irq(void) {
    // Check if interrupts are enabled
    if (!irq_is_enabled()) {
        puts("IRQ is not enabled but required");
        return;
    }
    
    // Disable IRQ and check
    volatile unsigned previous_state = irq_disable();
    if (irq_is_enabled()) {
        puts("Failed to disable IRQ");
    } else {
        puts("IRQ successfully disabled");
    }

    // Restore IRQ and check
    irq_restore(previous_state);
    if (irq_is_enabled()) {
        puts("IRQ successfully restored");
    } else {
        puts("Failed to restore IRQ");
    }
}

int main(void) {
    puts("Starting interrupt handling test");
    
    test_irq();
    
    return 0;
}