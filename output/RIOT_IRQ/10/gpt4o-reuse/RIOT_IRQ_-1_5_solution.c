
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define IRQ_LINE 11

volatile int interrupt_occurred = 0;

void irq_handler(void *arg)
{
    (void)arg;
    interrupt_occurred = 1;
    printf("Interrupt occurred!\n");
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Register the interrupt handler
    irq_disable(IRQ_LINE);
    irq_set_handler(IRQ_LINE, irq_handler, NULL);
    irq_enable(IRQ_LINE);

    // Simulate an interrupt trigger
    xtimer_sleep(1); // Wait for a second to ensure the interrupt is enabled
    irq_trigger(IRQ_LINE);

    // Wait for the interrupt to be handled
    while (!interrupt_occurred) {
        xtimer_usleep(100);
    }

    printf("Interrupt handling test completed.\n");

    return 0;
}
