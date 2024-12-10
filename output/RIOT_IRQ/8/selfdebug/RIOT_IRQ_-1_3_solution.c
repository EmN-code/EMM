#include "periph/irq.h"
#include "log_module.h"
#include "kernel_defines.h"
#include "embUnit/embUnit.h"

#include <stdio.h>
#include <stdatomic.h>

atomic_bool irq_disabled;

void irq_handler(void) {
    printf("Interrupt occurred!\n");
    atomic_store(&irq_disabled, true);  // Set the atomic variable to true when interrupt is triggered
}

void test_irq_handling(void) {
    irq_disable(); // Simulate disabling of interrupts
    atomic_store(&irq_disabled, false); // Set the atomic variable to false initially
    irq_set(irq_handler); // Set the handler for the interrupt
    irq_enable(); // Re-enable interrupts and check whether the handler runs
    
    irq_restore(); // Restore the interrupts

    TEST_ASSERT_TRUE(atomic_load(&irq_disabled)); // Test that the interrupt set the atomic variable as expected
}

int main(void) {
    printf("Starting IRQ Handling Test\n");
    test_irq_handling();
    return 0;
}