
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

    printf("Setting up interrupt handler...\n");
    irq_set_handler(IRQ_NUM, interrupt_handler, NULL);

    printf("Enabling interrupts...\n");
    irq_restore(state);

    while (1) {
        if (atomic_load(&atomic_var) == 1) {
            printf("Interrupt handled correctly.\n");
            break;
        }
    }

    return 0;
}
