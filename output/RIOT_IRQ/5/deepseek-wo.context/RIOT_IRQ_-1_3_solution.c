
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int_t atomic_var;

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_store(&atomic_var, 1);
}

int main(void)
{
    atomic_init(&atomic_var, 0);

    printf("Disabling interrupts...\n");
    unsigned state = irq_disable();

    printf("Setting interrupt handler...\n");
    irq_set_handler(interrupt_handler, NULL);

    printf("Restoring interrupts...\n");
    irq_restore(state);

    printf("Waiting for interrupt...\n");
    while (atomic_load(&atomic_var) == 0) {
        thread_yield();
    }

    printf("Interrupt received!\n");
    return 0;
}
