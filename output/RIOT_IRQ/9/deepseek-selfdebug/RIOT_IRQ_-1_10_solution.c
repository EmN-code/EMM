
#include <stdio.h>
#include <irq.h>
#include <atomic_utils.h>

atomic_int_t atomic_var = ATOMIC_INIT(0);

void irq_handler(void *arg)
{
    atomic_store(&atomic_var, 1);
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Set the IRQ handler
    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Modify the atomic variable
    atomic_store(&atomic_var, 0);

    // Restore interrupts
    irq_restore(state);

    // Trigger the interrupt
    irq_set(IRQ_NUM);

    // Wait for the interrupt to be handled
    while (atomic_load(&atomic_var) == 0) {
        ; // Busy wait
    }

    printf("Interrupt handled, atomic variable value: %d\n", atomic_load(&atomic_var));

    return 0;
}
