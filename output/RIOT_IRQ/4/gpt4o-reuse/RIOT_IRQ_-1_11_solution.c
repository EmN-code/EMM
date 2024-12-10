
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

    // Attach the interrupt handler
    if (irq_attach(IRQ_LINE, irq_handler, NULL) < 0) {
        printf("Failed to attach interrupt handler\n");
        return 1;
    }

    // Disable interrupts
    unsigned state = irq_disable();

    // Modify the atomic variable
    atomic_var = 0;

    // Restore interrupts
    irq_restore(state);

    // Wait for the interrupt to be handled
    xtimer_sleep(1);

    // Check if the interrupt handler modified the atomic variable
    if (atomic_var == 1) {
        printf("Interrupt handling test passed\n");
    } else {
        printf("Interrupt handling test failed\n");
    }

    // Detach the interrupt handler
    irq_detach(IRQ_LINE);

    return 0;
}
