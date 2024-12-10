
#include <stdio.h>
#include <stdint.h>
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

    unsigned state = irq_disable();
    atomic_store(&atomic_var, 2);
    irq_restore(state);

    while (1) {
        if (atomic_load(&atomic_var) == 1) {
            printf("Interrupt was handled correctly.\n");
            break;
        }
    }

    return 0;
}
