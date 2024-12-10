
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define IRQ_LINE 11

static volatile int atomic_var = 0;

static void irq_handler(void *arg)
{
    (void)arg;
    atomic_var = 1; // Set atomic variable to indicate interrupt occurred
    printf("Interrupt occurred\n");
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Register the interrupt handler
    irq_disable(IRQ_LINE);
    irq_set_handler(IRQ_LINE, irq_handler, NULL);
    irq_enable(IRQ_LINE);

    // Test disabling and enabling interrupts
    irq_disable(IRQ_LINE);
    printf("Interrupt disabled\n");
    xtimer_sleep(2); // Sleep for 2 seconds
    irq_enable(IRQ_LINE);
    printf("Interrupt enabled\n");

    // Wait for interrupt to occur
    while (atomic_var == 0) {
        xtimer_usleep(100); // Sleep for 100 microseconds
    }

    printf("Interrupt handling test completed\n");
    return 0;
}
