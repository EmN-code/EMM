
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

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
    irq_set_handler(IRQ_LINE, irq_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Perform some operations while interrupts are disabled
    printf("Interrupts are disabled\n");
    xtimer_sleep(2);  // Simulate some work

    // Restore interrupts to the previous state
    irq_restore(state);
    printf("Interrupts are restored\n");

    // Wait for the interrupt to occur
    while (atomic_var == 0) {
        xtimer_usleep(100);  // Small delay to avoid busy-waiting
    }

    printf("Interrupt handling test completed\n");
    return 0;
}
