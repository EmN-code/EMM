
#include <stdio.h>
#include <irq.h>
#include <atomic_utils.h>

atomic_int_t atomic_var;

void custom_irq_handler(void)
{
    printf("Interrupt occurred!\n");
    atomic_store(&atomic_var, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Initialize the atomic variable
    atomic_init(&atomic_var, 0);

    // Set the custom IRQ handler
    irq_set_handler(custom_irq_handler);

    // Disable interrupts
    unsigned state = irq_disable();

    // Modify the atomic variable while interrupts are disabled
    atomic_store(&atomic_var, 2);

    // Restore interrupts
    irq_restore(state);

    // Check the value of the atomic variable
    int value = atomic_load(&atomic_var);
    printf("Atomic variable value: %d\n", value);

    return 0;
}
