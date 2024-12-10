
#include <stdio.h>
#include "xtimer.h"
#include "irq.h"
#include "thread.h"

volatile int atomic_var = 0;

void interrupt_handler(void *arg)
{
    (void)arg;
    printf("Interrupt triggered!\n");
    atomic_var = 1;
}

int main(void)
{
    printf("Starting interrupt handling test on ESP32 with RIOT...\n");

    // Register the interrupt handler
    irq_disable();
    irq_set_handler(0, interrupt_handler, NULL);
    irq_enable();

    // Simulate an interrupt by setting a flag
    irq_set(0);

    // Wait for the interrupt to be handled
    xtimer_sleep(1);

    // Check if the atomic variable was set correctly
    if (atomic_var == 1) {
        printf("Interrupt handling and atomic variable update successful!\n");
    } else {
        printf("Interrupt handling failed!\n");
    }

    return 0;
}
