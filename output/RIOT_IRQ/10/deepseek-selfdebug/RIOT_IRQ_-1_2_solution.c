
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_types.h>

atomic_int atomic_var;

void interrupt_handler(void *arg)
{
    (void)arg;
    printf("Interrupt occurred!\n");
    atomic_store(&atomic_var, 1);
}

int main(void)
{
    atomic_store(&atomic_var, 0);

    printf("Disabling interrupts...\n");
    unsigned state = irq_disable();

    printf("Setting interrupt handler...\n");
    irq_set_handler(0, interrupt_handler, NULL);

    printf("Enabling interrupts...\n");
    irq_restore(state);

    printf("Waiting for interrupt...\n");
    while (atomic_load(&atomic_var) == 0) {
        thread_yield();
    }

    printf("Interrupt handled successfully!\n");

    return 0;
}