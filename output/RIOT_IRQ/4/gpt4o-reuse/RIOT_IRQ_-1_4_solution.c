
#include <stdio.h>
#include "irq.h"
#include "thread.h"
#include "xtimer.h"

#define IRQ_LINE (11)  // Assuming IRQ line 11 for ESP32

static volatile int atomic_var = 0;

static void irq_handler(void *arg)
{
    (void)arg;
    atomic_var = 1;  // Set atomic variable to indicate interrupt occurred
    printf("Interrupt occurred\n");
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Initialize the interrupt line
    irq_init();

    // Set the interrupt handler for the specified IRQ line
    irq_set(IRQ_LINE, irq_handler, NULL);

    // Disable the interrupt
    irq_disable(IRQ_LINE);

    // Simulate some work
    xtimer_sleep(2);

    // Check if the interrupt occurred while disabled
    if (atomic_var == 1) {
        printf("Error: Interrupt occurred while disabled\n");
    } else {
        printf("Interrupt did not occur while disabled\n");
    }

    // Enable the interrupt
    irq_enable(IRQ_LINE);

    // Simulate some work
    xtimer_sleep(2);

    // Check if the interrupt occurred after enabling
    if (atomic_var == 1) {
        printf("Interrupt occurred after enabling\n");
    } else {
        printf("Error: Interrupt did not occur after enabling\n");
    }

    // Clear the interrupt
    irq_clear(IRQ_LINE);

    printf("Interrupt handling test completed\n");

    return 0;
}
