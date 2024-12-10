
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

    unsigned state = irq_disable();
    if (atomic_load(&atomic_var) == 0) {
        printf("Interrupts were disabled correctly.\n");
    } else {
        printf("Interrupts were not disabled correctly.\n");
    }
    irq_restore(state);

    return 0;
}
