
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define IRQ_LINE (11)  // Assuming IRQ line 11 is available on ESP32

static volatile int atomic_var = 0;

static void irq_handler(void *arg)
{
    (void)arg;
    atomic_var = 1;  // Set atomic variable to indicate interrupt occurred
    printf("Interrupt occurred\n");
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Initialize the interrupt line
    irq_disable(IRQ_LINE);
    irq_set(IRQ_LINE, IRQ_PRIO_MEDIUM, irq_handler, NULL);
    irq_enable(IRQ_LINE);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        xtimer_sleep(1);
        printf("Working... %d\n", i);
    }

    // Disable the interrupt
    irq_disable(IRQ_LINE);
    printf("Interrupt disabled\n");

    // Simulate some more work
    for (int i = 0; i < 5; i++) {
        xtimer_sleep(1);
        printf("Working after disable... %d\n", i);
    }

    // Re-enable the interrupt
    irq_enable(IRQ_LINE);
    printf("Interrupt re-enabled\n");

    // Simulate some more work
    for (int i = 0; i < 5; i++) {
        xtimer_sleep(1);
        printf("Working after re-enable... %d\n", i);
    }

    // Check the atomic variable to verify if the interrupt was handled
    if (atomic_var) {
        printf("Interrupt was handled correctly\n");
    } else {
        printf("Interrupt was not handled\n");
    }

    return 0;
}
