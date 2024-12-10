
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

    printf("Disabling interrupts...\n");
    irq_disable();

    printf("Setting IRQ handler...\n");
    irq_set_handler(IRQ_NUM, irq_handler);

    printf("Triggering IRQ...\n");
    irq_trigger(IRQ_NUM);

    printf("Restoring interrupts...\n");
    irq_restore(irq_enable());

    int value = atomic_load(&atomic_var);
    printf("Atomic variable value: %d\n", value);

    return 0;
}
