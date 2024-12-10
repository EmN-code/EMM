
#include <stdio.h>
#include <stdint.h>
#include "irq.h"
#include "thread.h"
#include "xtimer.h"

#define IRQ_NUM 0  // Define the IRQ number to use

volatile uint32_t atomic_var = 0;  // Use a volatile uint32_t as an atomic variable

void irq_handler(void *arg)
{
    (void)arg;  // Unused parameter
    atomic_var = 1;  // Set the atomic variable to 1 when the interrupt is triggered
}

int main(void)
{
    // Initialize the atomic variable
    atomic_var = 0;

    // Set the interrupt handler
    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Modify the atomic variable while interrupts are disabled
    atomic_var = 2;

    // Check the atomic variable
    if (atomic_var == 2) {
        printf("Interrupts were disabled correctly.\n");
    } else {
        printf("Interrupts were not disabled correctly.\n");
    }

    // Restore interrupts
    irq_restore(state);

    // Trigger the interrupt
    irq_trigger(IRQ_NUM);

    // Wait for a short period to allow the interrupt handler to run
    xtimer_sleep(1);

    // Check the atomic variable again
    if (atomic_var == 1) {
        printf("Interrupts were restored correctly.\n");
    } else {
        printf("Interrupts were not restored correctly.\n");
    }

    return 0;
}
