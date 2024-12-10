
#include <stdio.h>
#include "irq.h"
#include "thread.h"
#include "xtimer.h"

#define IRQ_LINE (11)  // Assuming IRQ line 11 is available on ESP32

volatile unsigned int atomic_var = 0;

void irq_handler(void *arg)
{
    (void)arg;
    printf("Interrupt triggered!\n");
    atomic_var = 1;  // Set atomic variable to indicate interrupt occurred
}

int main(void)
{
    printf("Starting interrupt handling test on ESP32...\n");

    // Disable interrupts initially
    unsigned state = irq_disable();

    // Attach the interrupt handler
    irq_attach(IRQ_LINE, irq_handler, NULL);

    // Enable interrupts
    irq_restore(state);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        xtimer_sleep(1);
        printf("Working... %d\n", i);
    }

    // Check if the interrupt was triggered
    if (atomic_var) {
        printf("Interrupt was successfully handled!\n");
    } else {
        printf("Interrupt was not triggered.\n");
    }

    // Detach the interrupt handler
    irq_disable();
    irq_detach(IRQ_LINE);

    return 0;
}
