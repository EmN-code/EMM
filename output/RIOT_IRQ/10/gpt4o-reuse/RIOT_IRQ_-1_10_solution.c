
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define IRQ_LINE 11

volatile int atomic_var = 0;

void irq_handler(void *arg)
{
    (void)arg;
    atomic_var = 1; // Set atomic variable to indicate interrupt occurred
    printf("Interrupt handled\n");
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Register the interrupt handler
    irq_set_handler(IRQ_LINE, irq_handler, NULL);

    // Enable the interrupt
    irq_enable(IRQ_LINE);

    // Disable the interrupt to test atomic variable
    irq_disable(IRQ_LINE);
    atomic_var = 0; // Reset atomic variable
    xtimer_sleep(2); // Simulate some work
    if (atomic_var == 0) {
        printf("Interrupts were disabled correctly\n");
    } else {
        printf("Interrupts were not disabled correctly\n");
    }

    // Re-enable the interrupt
    irq_enable(IRQ_LINE);

    // Wait for interrupt to occur
    xtimer_sleep(10);
    if (atomic_var == 1) {
        printf("Interrupts were enabled correctly\n");
    } else {
        printf("Interrupts were not enabled correctly\n");
    }

    // Disable the interrupt again before exiting
    irq_disable(IRQ_LINE);

    printf("End of interrupt handling test\n");
    return 0;
}
