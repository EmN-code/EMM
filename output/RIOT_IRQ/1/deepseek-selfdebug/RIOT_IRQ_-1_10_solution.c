
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_t atomic_var = ATOMIC_INIT(0);

void irq_handler(void *arg)
{
    (void)arg;
    atomic_store(&atomic_var, 1);
}

int main(void)
{
    puts("RIOT interrupt handling example");

    // Set the interrupt handler
    irq_set_handler(0, irq_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Trigger the interrupt
    irq_trigger(0);

    // Restore interrupts
    irq_restore(state);

    // Check the atomic variable
    if (atomic_load(&atomic_var) == 1) {
        puts("Interrupt was handled correctly");
    } else {
        puts("Interrupt was not handled");
    }

    return 0;
}
