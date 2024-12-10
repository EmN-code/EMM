
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define IRQ_LINE 11

static volatile int atomic_var = 0;

static void irq_handler(void *arg)
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

    // Verify that the interrupt is disabled
    if (atomic_var == 0) {
        printf("Interrupt is disabled, atomic_var is still 0\n");
    } else {
        printf("Error: Interrupt is not disabled, atomic_var is %d\n", atomic_var);
    }

    // Enable the interrupt
    irq_enable(IRQ_LINE);

    // Simulate an interrupt by setting the atomic variable
    atomic_var = 1;

    // Verify that the interrupt is enabled
    if (atomic_var == 1) {
        printf("Interrupt is enabled, atomic_var is 1\n");
    } else {
        printf("Error: Interrupt is not enabled, atomic_var is %d\n", atomic_var);
    }

    // Clear the interrupt
    irq_clear(IRQ_LINE);

    printf("Interrupt handling test completed\n");

    return 0;
}
