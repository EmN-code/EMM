
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
    printf("Starting interrupt test\n");

    // Initialize the IRQ line
    irq_init();

    // Set the IRQ handler
    irq_set(IRQ_LINE, irq_handler, NULL);

    // Disable the IRQ
    irq_disable(IRQ_LINE);

    // Simulate some work
    xtimer_sleep(2);

    // Enable the IRQ
    irq_enable(IRQ_LINE);

    // Simulate some work
    xtimer_sleep(2);

    // Check if the interrupt occurred
    if (interrupt_occurred) {
        printf("Interrupt was successfully handled\n");
    } else {
        printf("Interrupt did not occur\n");
    }

    // Clear the IRQ
    irq_clear(IRQ_LINE);

    return 0;
}
