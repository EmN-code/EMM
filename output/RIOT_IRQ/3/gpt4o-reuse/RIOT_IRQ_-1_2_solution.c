
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define IRQ_LINE (11)  // Assuming IRQ line 11 is available on ESP32

volatile int atomic_var = 0;

void irq_handler(void *arg)
{
    (void)arg;
    atomic_var = 1;  // Set atomic variable to indicate interrupt occurred
    printf("Interrupt handled\n");
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Register the interrupt handler
    if (irq_disable(IRQ_LINE) != 0) {
        printf("Failed to disable IRQ\n");
        return 1;
    }

    if (irq_set(IRQ_LINE, (irq_handler_t)irq_handler, NULL) != 0) {
        printf("Failed to set IRQ handler\n");
        return 1;
    }

    if (irq_enable(IRQ_LINE) != 0) {
        printf("Failed to enable IRQ\n");
        return 1;
    }

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        xtimer_sleep(1);  // Sleep for 1 second
        printf("Working... %d\n", i);
    }

    // Disable the interrupt
    if (irq_disable(IRQ_LINE) != 0) {
        printf("Failed to disable IRQ\n");
        return 1;
    }

    // Check if the interrupt was handled
    if (atomic_var == 1) {
        printf("Interrupt was handled correctly\n");
    } else {
        printf("Interrupt was not handled\n");
    }

    printf("End of interrupt handling test\n");
    return 0;
}
