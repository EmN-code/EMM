
#include <stdio.h>
#include <xtimer.h>
#include <irq.h>
#include <thread.h>

volatile int atomic_var = 0;

void interrupt_handler(void *arg)
{
    (void)arg;
    printf("Interrupt triggered!\n");
    atomic_var = 1; // Set atomic variable to indicate interrupt occurred
}

int main(void)
{
    printf("Starting interrupt handling test on ESP32 with RIOT...\n");

    // Register the interrupt handler
    irq_disable(); // Disable interrupts globally
    irq_set(0, interrupt_handler, NULL); // Set interrupt handler for IRQ 0
    irq_enable(); // Enable interrupts globally

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        printf("Working... %d\n", i);
        xtimer_sleep(1); // Sleep for 1 second

        // Check if the interrupt has been triggered
        if (atomic_var == 1) {
            printf("Interrupt detected! Disabling and restoring interrupts...\n");

            // Disable interrupts
            irq_disable();
            printf("Interrupts disabled. Atomic variable: %d\n", atomic_var);

            // Simulate some work while interrupts are disabled
            xtimer_sleep(2);

            // Restore interrupts
            irq_enable();
            printf("Interrupts restored.\n");

            // Reset atomic variable
            atomic_var = 0;
        }
    }

    printf("Interrupt handling test completed.\n");
    return 0;
}
