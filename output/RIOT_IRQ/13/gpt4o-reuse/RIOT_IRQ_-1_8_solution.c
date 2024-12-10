
#include <stdio.h>
#include "irq.h"
#include "thread.h"
#include "xtimer.h"

#define IRQ_LINE (11)  // Assuming IRQ line 11 for ESP32

volatile int atomic_var = 0;

void irq_handler(void *arg)
{
    (void)arg;
    atomic_var = 1;  // Set atomic variable to indicate interrupt occurred
    printf("Interrupt occurred\n");
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Register the interrupt handler
    irq_disable();
    irq_set(IRQ_LINE, irq_handler, NULL);
    irq_enable();

    // Test the interrupt handling by disabling and enabling interrupts
    irq_disable();
    printf("Interrupts disabled\n");
    xtimer_sleep(2);  // Sleep for 2 seconds to simulate some work
    irq_enable();
    printf("Interrupts enabled\n");

    // Wait for the interrupt to occur
    while (atomic_var == 0) {
        xtimer_usleep(100);  // Small sleep to avoid busy-waiting
    }

    printf("Interrupt handling test completed\n");
    return 0;
}
