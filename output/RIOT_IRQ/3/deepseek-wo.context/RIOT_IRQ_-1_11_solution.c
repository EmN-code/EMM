
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
    atomic_init(&atomic_var, 0);

    irq_set_handler(0, irq_handler, NULL);
    irq_trigger(0);

    while (1) {
        if (atomic_load(&atomic_var) == 1) {
            printf("Interrupt handled successfully!\n");
            break;
        }
    }

    return 0;
}
