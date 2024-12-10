
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define IRQ_LINE 11  // Assuming IRQ line 11 is available on ESP32

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

    // Register the interrupt handler
    irq_disable(IRQ_LINE);
    irq_set(IRQ_LINE, irq_handler, NULL);
    irq_enable(IRQ_LINE);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        xtimer_sleep(1);  // Sleep for 1 second
        printf("Main thread running, atomic_var: %d\n", atomic_var);
    }

    // Disable the interrupt
    irq_disable(IRQ_LINE);
    printf("Interrupt disabled, atomic_var: %d\n", atomic_var);

    // Re-enable the interrupt
    irq_enable(IRQ_LINE);
    printf("Interrupt re-enabled\n");

    // Simulate more work
    for (int i = 0; i < 10; i++) {
        xtimer_sleep(1);  // Sleep for 1 second
        printf("Main thread running, atomic_var: %d\n", atomic_var);
    }

    printf("Interrupt handling test completed\n");
    return 0;
}
