#include <kernel_types.h>
#include <irq.h>
#include <stdio.h>

volatile unsigned irq_state;
volatile int interrupt_handled;

void interrupt_test_handler(void) {
    interrupt_handled += 1;
}

int main(void) {
    irq_state = irq_disable();
    printf("Interrupts disabled!\n");

    // Simulate interrupt handling
    for(int i = 0; i < 1000; i++) {
        if (i % 100 == 0) {
            interrupt_test_handler();
        }
    }
    printf("Interrupts handled: %d\n", interrupt_handled);

    irq_restore(irq_state);
    printf("Interrupts restored!\n");

    return 0;
}