
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
    printf("Interrupt handled\n");
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Attach the interrupt handler
    if (irq_attach(IRQ_LINE, irq_handler, 0) < 0) {
        printf("Failed to attach interrupt handler\n");
        return 1;
    }

    // Disable interrupts
    unsigned state = irq_disable();

    // Perform some atomic operation
    atomic_var = 0;

    // Restore interrupts
    irq_restore(state);

    // Wait for the interrupt to be handled
    while (atomic_var == 0) {
        xtimer_sleep(1);
    }

    printf("Interrupt was successfully handled\n");

    // Detach the interrupt handler
    irq_detach(IRQ_LINE);

    return 0;
}
