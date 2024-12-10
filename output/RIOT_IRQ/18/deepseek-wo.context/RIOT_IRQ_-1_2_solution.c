
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int_t atomic_var;

void interrupt_handler(void *arg)
{
    (void)arg;
    printf("Interrupt triggered!\n");
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

    printf("Waiting for interrupt...\n");
    while (atomic_load(&atomic_var) != 1) {
        thread_yield();
    }

    printf("Interrupt handled, atomic variable value: %d\n", atomic_load(&atomic_var));

    return 0;
}
