
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int_t atomic_var;

void irq_handler(void *arg)
{
    (void)arg;
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

    thread_sleep(1000000);

    int value = atomic_load(&atomic_var);
    printf("Atomic variable value: %d\n", value);

    return 0;
}