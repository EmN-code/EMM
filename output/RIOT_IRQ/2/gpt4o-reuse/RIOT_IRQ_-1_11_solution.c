
#include <stdio.h>
#include "irq.h"
#include "thread.h"
#include "xtimer.h"

#define IRQ_LINE (11)  // Assuming IRQ line 11 for ESP32

volatile int atomic_var = 0;

void irq_handler(void *arg)
{
    (void)arg;
    atomic_var = 1;  // Set atomic variable to verify interrupt handling
    printf("Interrupt handled, atomic_var set to 1\n");
}

int main(void)
{
    printf("Starting interrupt handling test on ESP32\n");

    // Initialize the interrupt line
    irq_init();

    // Set the interrupt handler for the specified IRQ line
    irq_set(IRQ_LINE, irq_handler, NULL);

    // Disable the interrupt to demonstrate disabling
    irq_disable(IRQ_LINE);
    printf("Interrupt disabled\n");

    // Simulate some work
    xtimer_sleep(2);

    // Enable the interrupt to demonstrate enabling
    irq_enable(IRQ_LINE);
    printf("Interrupt enabled\n");

    // Simulate some work
    xtimer_sleep(2);

    // Check the atomic variable to verify interrupt handling
    if (atomic_var == 1) {
        printf("Interrupt was successfully handled\n");
    } else {
        printf("Interrupt was not handled\n");
    }

    // Clear the interrupt handler
    irq_clear(IRQ_LINE);
    printf("Interrupt handler cleared\n");

    return 0;
}
