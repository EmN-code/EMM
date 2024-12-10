
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int atomic_var = ATOMIC_INIT(0);

void interrupt_handler(void)
{
    printf("Interrupt occurred!\n");
    atomic_store(&atomic_var, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Disable interrupts
    unsigned state = irq_disable();

    // Simulate some work
    printf("Disabling interrupts...\n");

    // Restore interrupts
    irq_restore(state);

    // Register the interrupt handler
    irq_set_handler(IRQ_NUM, interrupt_handler);

    // Enable interrupts
    irq_enable(IRQ_NUM);

    // Simulate some work
    printf("Doing some work...\n");

    // Check the atomic variable
    if (atomic_load(&atomic_var) == 1) {
        printf("Interrupt was handled correctly.\n");
    } else {
        printf("Interrupt was not handled.\n");
    }

    return 0;
}
