
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
    irq_enable(IRQ_LINE);
    irq_set(IRQ_LINE, irq_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Modify the atomic variable
    atomic_var = 0;
    printf("Atomic variable set to 0 with interrupts disabled\n");

    // Restore interrupts
    irq_restore(state);

    // Wait for an interrupt to occur
    xtimer_sleep(1);

    // Check if the interrupt handler modified the atomic variable
    if (atomic_var == 1) {
        printf("Interrupt was handled correctly\n");
    } else {
        printf("Interrupt was not handled\n");
    }

    // Cleanup
    irq_disable(IRQ_LINE);
    irq_clear(IRQ_LINE);

    return 0;
}
