
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

    // Test disabling and restoring interrupts
    irq_disable();
    atomic_var = 0;  // Reset atomic variable
    irq_enable();

    // Wait for interrupt to occur
    xtimer_sleep(1);  // Sleep for 1 second

    // Check if interrupt was handled
    if (atomic_var == 1) {
        printf("Interrupt was handled correctly\n");
    } else {
        printf("Interrupt was not handled\n");
    }

    // Clean up
    irq_disable();
    irq_clear(IRQ_LINE);

    return 0;
}
