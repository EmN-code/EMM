
#include <stdio.h>
#include <stdint.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define IRQ_LINE (11)  // Assuming IRQ line 11 is available on ESP32

static volatile uint32_t atomic_var = 0;

static void irq_handler(void *arg)
{
    (void)arg;
    printf("Interrupt triggered!\n");
    atomic_var = 1;  // Set atomic variable to indicate interrupt occurred
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
        printf("Working... %d\n", i);
        xtimer_sleep(1);  // Sleep for 1 second

        // Check if interrupt was handled
        if (atomic_var) {
            printf("Interrupt was handled correctly!\n");
            atomic_var = 0;  // Reset atomic variable
        }
    }

    // Clean up
    irq_disable(IRQ_LINE);
    irq_clear(IRQ_LINE);

    printf("Interrupt handling test completed\n");
    return 0;
}
