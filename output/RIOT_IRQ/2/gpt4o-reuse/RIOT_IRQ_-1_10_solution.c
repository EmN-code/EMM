
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define IRQ_LINE (11)  // Assuming IRQ line 11 for ESP32

volatile int atomic_var = 0;

void irq_handler(void *arg)
{
    (void)arg;
    atomic_var = 1;  // Set atomic variable to indicate interrupt occurred
    printf("Interrupt occurred\n");
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Initialize the interrupt line
    irq_init();

    // Set the interrupt handler for the specified IRQ line
    irq_set(IRQ_LINE, irq_handler, NULL);

    // Disable the interrupt
    irq_disable(IRQ_LINE);

    // Perform some operations while the interrupt is disabled
    for (int i = 0; i < 10; i++) {
        printf("Atomic variable: %d\n", atomic_var);
        xtimer_usleep(100000);  // Sleep for 100 ms
    }

    // Enable the interrupt
    irq_enable(IRQ_LINE);

    // Perform some operations while the interrupt is enabled
    for (int i = 0; i < 10; i++) {
        printf("Atomic variable: %d\n", atomic_var);
        xtimer_usleep(100000);  // Sleep for 100 ms
    }

    // Clear the interrupt
    irq_clear(IRQ_LINE);

    printf("End of interrupt handling test\n");
    return 0;
}
