
#include <stdio.h>
#include "irq.h"
#include "atomic_utils.h"

atomic_t flag = ATOMIC_INIT(0);

void interrupt_handler(void)
{
    atomic_store(&flag, 1);
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Disable interrupts
    unsigned state = irq_disable();

    // Set the interrupt handler
    irq_set_handler(IRQ_NUM, interrupt_handler);

    // Enable the interrupt
    irq_enable(IRQ_NUM);

    // Restore interrupts
    irq_restore(state);

    // Simulate some work
    for (int i = 0; i < 1000000; i++) {
        // Busy loop
    }

    // Check the flag
    if (atomic_load(&flag) == 1) {
        printf("Interrupt was handled correctly\n");
    } else {
        printf("Interrupt was not handled\n");
    }

    return 0;
}
