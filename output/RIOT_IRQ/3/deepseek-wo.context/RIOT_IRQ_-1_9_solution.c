
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int_t atomic_var;

void irq_handler(void *arg)
{
    atomic_store(&atomic_var, 1);
}

int main(void)
{
    atomic_init(&atomic_var, 0);

    printf("Disabling interrupts...\n");
    unsigned state = irq_disable();

    printf("Setting IRQ handler...\n");
    irq_set_handler(0, irq_handler, NULL);

    printf("Restoring interrupts...\n");
    irq_restore(state);

    printf("Triggering IRQ...\n");
    irq_trigger(0);

    printf("Checking atomic variable...\n");
    if (atomic_load(&atomic_var) == 1) {
        printf("Interrupts were handled correctly.\n");
    } else {
        printf("Interrupts were not handled correctly.\n");
    }

    return 0;
}
