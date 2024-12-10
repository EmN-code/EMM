
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

    // Initialize the interrupt line
    irq_init();

    // Set the interrupt handler
    irq_set(IRQ_LINE, irq_handler, NULL);

    // Disable the interrupt
    irq_disable(IRQ_LINE);

    // Simulate some work
    xtimer_sleep(2);

    // Verify that the interrupt was not handled
    if (atomic_var == 0) {
        printf("Interrupt was disabled correctly\n");
    } else {
        printf("Error: Interrupt was not disabled\n");
    }

    // Enable the interrupt
    irq_enable(IRQ_LINE);

    // Simulate some work
    xtimer_sleep(2);

    // Verify that the interrupt was handled
    if (atomic_var == 1) {
        printf("Interrupt was enabled and handled correctly\n");
    } else {
        printf("Error: Interrupt was not enabled or handled\n");
    }

    // Clear the interrupt
    irq_clear(IRQ_LINE);

    printf("Interrupt handling test completed\n");

    return 0;
}
