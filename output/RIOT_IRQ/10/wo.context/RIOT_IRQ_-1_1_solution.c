#include <irq_arch.h>
#include <stdio.h>

volatile unsigned irq_status;

void test_irq_disable_and_restore(void) {
    irq_status = irq_disable();
    printf("Interrupts disabled. IRQ status: %u\n", irq_status);
    
    // Simulating some operation that requires interrupts to be disabled
    unsigned busy_work = 0;
    for (unsigned i = 0; i < 100000; ++i) {
        busy_work += i;
    }
    
    irq_restore(irq_status);
    printf("Interrupts restored.IRQ status: %u\n", irq_status);
}

int main(void) {
    printf("Starting IRQ test.\n");
    test_irq_disable_and_restore();
    printf("IRQ test completed.\n");
    
    return 0;
}