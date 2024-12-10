
#include <stdio.h>
#include <xtimer.h>
#include <irq.h>
#include <thread.h>
#include <periph/atomic_utils.h>

volatile int atomic_var = 0;

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_var++;
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Register an interrupt handler
    irq_set_handler(IRQ_NUM, interrupt_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Perform some atomic operations
    atomic_var = 1;

    // Restore interrupts
    irq_restore(state);

    // Wait for the interrupt to be handled
    xtimer_sleep(1);

    // Check the atomic variable
    if (atomic_var == 2) {
        printf("Interrupt handling and atomic operations worked correctly.\n");
    } else {
        printf("Interrupt handling or atomic operations failed.\n");
    }

    return 0;
}
