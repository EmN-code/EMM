
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define IRQ_LINE (11)  // Assuming IRQ line 11 for ESP32

static volatile int interrupt_occurred = 0;

void irq_handler(void *arg)
{
    (void)arg;
    interrupt_occurred = 1;
    printf("Interrupt occurred\n");
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Initialize the interrupt line
    irq_init();

    // Set the interrupt handler
    irq_set(IRQ_LINE, irq_handler, NULL);

    // Disable the interrupt
    unsigned state = irq_disable();

    // Verify that the interrupt is disabled
    if (interrupt_occurred) {
        printf("Error: Interrupt occurred while disabled\n");
    } else {
        printf("Interrupt is disabled correctly\n");
    }

    // Restore the interrupt state
    irq_restore(state);

    // Enable the interrupt
    state = irq_enable();

    // Wait for the interrupt to occur
    xtimer_sleep(5);  // Wait for 5 seconds

    // Verify that the interrupt occurred
    if (interrupt_occurred) {
        printf("Interrupt occurred as expected\n");
    } else {
        printf("Error: Interrupt did not occur\n");
    }

    // Clear the interrupt
    irq_clear(IRQ_LINE);

    printf("Interrupt handling test completed\n");

    return 0;
}
