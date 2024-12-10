
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int_t atomic_var;

void irq_handler(void *arg)
{
    (void)arg;
    printf("Interrupt triggered!\n");
    atomic_store(&atomic_var, 1);
}

int main(void)
{
    atomic_init(&atomic_var, 0);

    printf("Disabling interrupts...\n");
    irq_disable();

    printf("Setting interrupt handler...\n");
    irq_set_handler(0, irq_handler, NULL);

    printf("Enabling interrupts...\n");
    irq_enable();

    printf("Triggering interrupt...\n");
    irq_trigger(0);

    printf("Waiting for interrupt to be handled...\n");
    while (atomic_load(&atomic_var) == 0) {
        thread_yield();
    }

    printf("Interrupt handled, atomic variable value: %d\n", atomic_load(&atomic_var));

    return 0;
}
