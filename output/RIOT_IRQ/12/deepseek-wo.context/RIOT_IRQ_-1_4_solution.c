
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include <stdbool.h>

volatile bool interrupt_occurred = false;

void my_irq_handler(void *arg)
{
    (void)arg;
    interrupt_occurred = true;
}

int main(void)
{
    // Initialize the interrupt handler
    irq_set_handler(0, my_irq_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Perform some atomic operation
    interrupt_occurred = false;

    // Restore interrupts
    irq_restore(state);

    // Trigger the interrupt
    irq_trigger(0);

    // Wait for the interrupt to be handled
    xtimer_sleep(1);

    // Check if the interrupt was handled
    if (interrupt_occurred) {
        printf("Interrupt was handled correctly.\n");
    } else {
        printf("Interrupt was not handled.\n");
    }

    return 0;
}
