
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

    printf("Setting atomic variable to 2...\n");
    atomic_store(&atomic_var, 2);

    printf("Restoring interrupts...\n");
    irq_restore(state);

    printf("Triggering interrupt...\n");
    interrupt_handler();

    int value = atomic_load(&atomic_var);
    printf("Atomic variable value: %d\n", value);

    return 0;
}
