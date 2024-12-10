
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

    irq_enable();

    while (1) {
        if (atomic_load(&atomic_var) == 1) {
            printf("Interrupt handled successfully!\n");
            atomic_store(&atomic_var, 0);
        }
    }

    return 0;
}
