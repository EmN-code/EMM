
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int_t atomic_var;

void interrupt_handler(void)
{
    atomic_store(&atomic_var, 1);
}

int main(void)
{
    atomic_init(&atomic_var, 0);

    printf("Disabling interrupts...\n");
    unsigned state = irq_disable();

    printf("Setting atomic variable to 1...\n");
    atomic_store(&atomic_var, 1);

    printf("Restoring interrupts...\n");
    irq_restore(state);

    printf("Checking atomic variable...\n");
    int value = atomic_load(&atomic_var);

    if (value == 1) {
        printf("Interrupts were disabled and restored correctly.\n");
    } else {
        printf("Interrupts were not handled correctly.\n");
    }

    return 0;
}
