
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

    irq_set_handler(0, irq_handler, NULL);
    irq_set(0, 1);

    printf("Atomic variable before interrupt: %d\n", atomic_load(&atomic_var));

    irq_set(0, 0);

    printf("Atomic variable after interrupt: %d\n", atomic_load(&atomic_var));

    return 0;
}
