
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_types.h>

atomic_int_t atomic_var = ATOMIC_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    printf("Interrupt occurred!\n");
    atomic_store(&atomic_var, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Disable interrupts
    unsigned state = irq_disable();

    // Register the interrupt handler
    irq_set_handler(IRQ_NUM, interrupt_handler, NULL);

    // Simulate an interrupt
    irq_enable(IRQ_NUM);
    irq_trigger(IRQ_NUM);

    // Restore interrupts
    irq_restore(state);

    // Check the atomic variable
    if (atomic_load(&atomic_var) == 1) {
        printf("Interrupt handling successful!\n");
    } else {
        printf("Interrupt handling failed!\n");
    }

    return 0;
}
