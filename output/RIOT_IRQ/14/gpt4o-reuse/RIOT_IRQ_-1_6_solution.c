
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define IRQ_LINE (11)  // Assuming IRQ line 11 is available on ESP32

volatile int atomic_var = 0;

void irq_handler(void *arg)
{
    (void)arg;
    printf("Interrupt triggered!\n");
    atomic_var = 1;  // Set atomic variable to verify interrupt handling
}

int main(void)
{
    printf("Starting interrupt handling test on ESP32...\n");

    // Register the interrupt handler
    irq_disable(IRQ_LINE);
    irq_set_handler(IRQ_LINE, irq_handler, NULL);
    irq_enable(IRQ_LINE);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        printf("Working... %d\n", i);
        xtimer_sleep(1);  // Sleep for 1 second

        // Check if interrupt was handled
        if (atomic_var == 1) {
            printf("Interrupt was handled correctly!\n");
            atomic_var = 0;  // Reset atomic variable
        }
    }

    // Disable the interrupt
    irq_disable(IRQ_LINE);
    printf("Interrupt handling test completed.\n");

    return 0;
}
