
#include <stdio.h>
#include <irq.h>
#include <xtimer.h>
#include <thread.h>

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

    // Initialize the interrupt line
    irq_disable(IRQ_LINE);
    irq_set_handler(IRQ_LINE, irq_handler, NULL);
    irq_enable(IRQ_LINE);

    // Simulate an interrupt by setting the atomic variable
    atomic_var = 0;
    printf("Atomic variable set to 0\n");

    // Trigger the interrupt
    irq_trigger(IRQ_LINE);

    // Wait for the interrupt to be handled
    xtimer_usleep(10000); // Wait for 10ms

    // Check if the interrupt was handled correctly
    if (atomic_var == 1) {
        printf("Interrupt handling successful\n");
    } else {
        printf("Interrupt handling failed\n");
    }

    // Disable the interrupt line
    irq_disable(IRQ_LINE);

    return 0;
}
