
#include <stdio.h>
#include <irq.h>
#include <atomic_utils.h>

atomic_int_t atomic_var;

void irq_handler(void)
{
    atomic_store(&atomic_var, 1);
}

int main(void)
{
    atomic_init(&atomic_var, 0);

    // Set the IRQ handler
    irq_set_handler(irq_handler);

    // Disable interrupts
    unsigned state = irq_disable();

    // Perform some critical section code
    printf("Critical section: atomic_var = %d\n", atomic_load(&atomic_var));

    // Restore interrupts
    irq_restore(state);

    // Check the atomic variable after restoring interrupts
    printf("After restoring interrupts: atomic_var = %d\n", atomic_load(&atomic_var));

    return 0;
}
