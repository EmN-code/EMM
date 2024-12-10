
#include <stdio.h>
#include "irq.h"
#include "thread.h"
#include "xtimer.h"

#define IRQ_LINE (11)  // Assuming IRQ line 11 for ESP32

volatile int atomic_var = 0;

void irq_handler(void)
{
    atomic_var = 1;  // Set atomic variable to indicate interrupt occurred
    printf("Interrupt occurred\n");
}

int main(void)
{
    // Initialize the IRQ line and set the handler
    irq_disable(IRQ_LINE);
    irq_set(IRQ_LINE, irq_handler);
    irq_enable(IRQ_LINE);

    printf("Interrupt handler set and enabled\n");

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        xtimer_sleep(1);  // Sleep for 1 second
        printf("Working... %d\n", i);

        // Check if interrupt occurred
        if (atomic_var) {
            printf("Atomic variable set, interrupt was handled\n");
            atomic_var = 0;  // Reset atomic variable
        }
    }

    // Disable the IRQ line
    irq_disable(IRQ_LINE);
    printf("Interrupt disabled\n");

    return 0;
}
