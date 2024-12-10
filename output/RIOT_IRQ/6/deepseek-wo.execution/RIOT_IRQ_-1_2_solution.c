
#include "irq.h"
#include "event/thread.h"
#include "thread.h"

volatile unsigned int atomic_var = 0;

void irq_handler(void)
{
    atomic_var = 1;
}

int main(void)
{
    unsigned state = irq_disable();

    // Set the atomic variable to 0
    atomic_var = 0;

    // Restore the interrupt state
    irq_restore(state);

    // Enable the interrupt
    irq_enable();

    // Simulate an interrupt occurring
    irq_handler();

    // Check the atomic variable
    if (atomic_var == 1) {
        printf("Interrupt handling works correctly.\n");
    } else {
        printf("Interrupt handling failed.\n");
    }

    return 0;
}
