
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define IRQ_LINE 11

volatile int atomic_var = 0;

void irq_handler(void *arg)
{
    (void)arg;
    atomic_var = 1;
    printf("Interrupt handled, atomic_var set to 1\n");
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Register the interrupt handler
    irq_set_handler(IRQ_LINE, irq_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Perform some atomic operation
    atomic_var = 0;

    // Restore interrupts
    irq_restore(state);

    // Simulate some delay to allow interrupt to occur
    xtimer_sleep(1);

    // Check if the interrupt was handled
    if (atomic_var == 1) {
        printf("Interrupt was handled correctly\n");
    } else {
        printf("Interrupt was not handled\n");
    }

    return 0;
}
