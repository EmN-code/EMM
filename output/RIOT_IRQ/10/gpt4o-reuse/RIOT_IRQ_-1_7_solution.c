
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

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        printf("Working... %d\n", i);
        xtimer_sleep(1);
    }

    // Disable the interrupt
    irq_disable(IRQ_LINE);

    // Check if the interrupt was handled
    if (atomic_var == 1) {
        printf("Interrupt was successfully handled\n");
    } else {
        printf("Interrupt was not handled\n");
    }

    printf("End of interrupt handling test\n");
    return 0;
}
