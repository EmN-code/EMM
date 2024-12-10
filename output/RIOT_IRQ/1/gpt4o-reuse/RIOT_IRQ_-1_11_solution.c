
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

    // Perform some operations with interrupts disabled
    printf("Interrupts are disabled\n");
    xtimer_sleep(2);

    // Restore interrupts to their previous state
    irq_restore(state);
    printf("Interrupts are restored\n");

    // Wait for the interrupt to occur
    while (atomic_var == 0) {
        xtimer_usleep(100);
    }

    // Detach the interrupt handler
    irq_detach(IRQ_LINE);
    printf("Interrupt handler detached\n");

    printf("Interrupt handling test completed\n");
    return 0;
}
