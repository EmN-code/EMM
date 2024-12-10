
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

    irq_set_handler(IRQ_NUM, irq_handler);
    irq_set(IRQ_NUM, 1);

    printf("Disabling interrupts...\n");
    unsigned state = irq_disable();

    printf("Atomic variable before interrupt: %d\n", atomic_load(&atomic_var));

    printf("Triggering interrupt...\n");
    irq_set(IRQ_NUM, 0);

    printf("Atomic variable after interrupt: %d\n", atomic_load(&atomic_var));

    printf("Restoring interrupts...\n");
    irq_restore(state);

    return 0;
}
