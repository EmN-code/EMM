
#include <stdio.h>
#include "irq.h"
#include "thread.h"
#include "xtimer.h"

#define IRQ_LINE (11)  // Assuming IRQ line 11 for ESP32

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
    irq_disable();
    irq_set(IRQ_LINE, irq_handler, NULL);
    irq_enable();

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        xtimer_sleep(1);
        printf("Working... %d\n", i);

        // Check if interrupt occurred
        if (interrupt_occurred) {
            printf("Interrupt was handled correctly\n");
            interrupt_occurred = 0;  // Reset the flag
        }
    }

    // Cleanup
    irq_disable();
    irq_clear(IRQ_LINE);
    irq_enable();

    printf("End of interrupt handling test\n");
    return 0;
}
