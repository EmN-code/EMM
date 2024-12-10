
#include <stdio.h>
#include <irq.h>
#include <atomic_utils.h>

atomic_int_t atomic_var;

void irq_handler(void *arg)
{
    atomic_store(&atomic_var, 1);
}

int main(void)
{
    atomic_init(&atomic_var, 0);

    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    printf("Disabling interrupts...\n");
    irq_disable();

    printf("Atomic variable value: %d\n", atomic_load(&atomic_var));

    printf("Restoring interrupts...\n");
    irq_restore(irq_enable());

    printf("Atomic variable value after interrupt: %d\n", atomic_load(&atomic_var));

    return 0;
}
