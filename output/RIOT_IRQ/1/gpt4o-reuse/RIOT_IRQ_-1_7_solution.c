
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define IRQ_LINE (11)  // Assuming IRQ line 11 for ESP32

volatile int atomic_var = 0;

void irq_handler(void *arg)
{
    (void)arg;
    atomic_var++;
    printf("Interrupt handled, atomic_var incremented to: %d\n", atomic_var);
}

int main(void)
{
    printf("Starting interrupt handling test on ESP32\n");

    // Initialize the IRQ line and set the handler
    irq_init();
    irq_set(IRQ_LINE, irq_handler, NULL);

    // Disable and then enable the IRQ to test atomicity
    irq_disable(IRQ_LINE);
    printf("IRQ disabled, atomic_var: %d\n", atomic_var);

    // Simulate some work
    xtimer_sleep(2);

    irq_enable(IRQ_LINE);
    printf("IRQ enabled, atomic_var: %d\n", atomic_var);

    // Simulate some work
    xtimer_sleep(2);

    // Clear the IRQ line
    irq_clear(IRQ_LINE);
    printf("IRQ cleared, atomic_var: %d\n", atomic_var);

    return 0;
}
