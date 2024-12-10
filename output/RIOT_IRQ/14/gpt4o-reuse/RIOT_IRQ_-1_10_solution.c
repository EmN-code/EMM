
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define IRQ_LINE 11

static volatile int atomic_var = 0;

static void irq_handler(void *arg)
{
    (void)arg;
    atomic_var = 1; // Set atomic variable to indicate interrupt occurred
    printf("Interrupt occurred\n");
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Register the interrupt handler
    irq_set_handler(IRQ_LINE, irq_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Perform some operations while interrupts are disabled
    for (int i = 0; i < 1000000; i++) {
        // Simulate work
    }

    // Restore interrupts to their previous state
    irq_restore(state);

    // Wait for the interrupt to occur
    xtimer_sleep(1);

    // Check if the interrupt handler set the atomic variable
    if (atomic_var == 1) {
        printf("Interrupt was handled correctly\n");
    } else {
        printf("Interrupt was not handled\n");
    }

    // Unregister the interrupt handler
    irq_set_handler(IRQ_LINE, NULL, NULL);

    return 0;
}
