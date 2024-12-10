
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define IRQ_LINE 11  // Assuming IRQ line 11 is available on ESP32

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

    // Register the interrupt handler
    irq_disable(IRQ_LINE);
    irq_set(IRQ_LINE, irq_handler, NULL);
    irq_enable(IRQ_LINE);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        xtimer_sleep(1);  // Sleep for 1 second
        printf("Working... %d\n", i);
    }

    // Disable interrupts and check atomic variable
    irq_disable(IRQ_LINE);
    if (atomic_var == 1) {
        printf("Interrupt was handled while interrupts were enabled\n");
    } else {
        printf("Interrupt was not handled\n");
    }

    // Restore interrupts
    irq_enable(IRQ_LINE);

    // Simulate more work
    for (int i = 0; i < 10; i++) {
        xtimer_sleep(1);  // Sleep for 1 second
        printf("Working... %d\n", i);
    }

    // Final check of atomic variable
    if (atomic_var == 1) {
        printf("Interrupt was handled while interrupts were enabled\n");
    } else {
        printf("Interrupt was not handled\n");
    }

    printf("End of interrupt handling test\n");
    return 0;
}
