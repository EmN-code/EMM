
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define IRQ_LINE (11)  // Assuming IRQ line 11 is available on ESP32

volatile uint32_t atomic_var = 0;

void irq_handler(void *arg)
{
    (void)arg;
    atomic_var++;
    printf("Interrupt handled, atomic_var incremented to: %lu\n", (unsigned long)atomic_var);
}

int main(void)
{
    printf("Starting interrupt handling test on ESP32\n");

    // Register the interrupt handler
    irq_disable(IRQ_LINE);
    irq_set_handler(IRQ_LINE, irq_handler, NULL);
    irq_enable(IRQ_LINE);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        xtimer_sleep(1);  // Sleep for 1 second
        printf("Main thread running, atomic_var: %lu\n", (unsigned long)atomic_var);
    }

    // Disable the interrupt
    irq_disable(IRQ_LINE);
    printf("Interrupt disabled, atomic_var: %lu\n", (unsigned long)atomic_var);

    // Simulate some more work
    for (int i = 0; i < 5; i++) {
        xtimer_sleep(1);  // Sleep for 1 second
        printf("Main thread running, atomic_var: %lu\n", (unsigned long)atomic_var);
    }

    // Re-enable the interrupt
    irq_enable(IRQ_LINE);
    printf("Interrupt re-enabled, atomic_var: %lu\n", (unsigned long)atomic_var);

    // Simulate some more work
    for (int i = 0; i < 5; i++) {
        xtimer_sleep(1);  // Sleep for 1 second
        printf("Main thread running, atomic_var: %lu\n", (unsigned long)atomic_var);
    }

    printf("Interrupt handling test completed\n");
    return 0;
}
