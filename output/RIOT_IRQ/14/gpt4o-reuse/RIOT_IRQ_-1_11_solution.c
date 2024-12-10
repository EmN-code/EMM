
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
    printf("Interrupt occurred\n");
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Register the interrupt handler
    irq_disable(IRQ_LINE);
    irq_set_handler(IRQ_LINE, irq_handler, NULL);
    irq_enable(IRQ_LINE);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        xtimer_sleep(1);
        if (interrupt_occurred) {
            printf("Interrupt was handled correctly\n");
            interrupt_occurred = 0; // Reset the flag
        }
    }

    // Disable the interrupt
    irq_disable(IRQ_LINE);
    printf("Interrupt handling test completed\n");

    return 0;
}
