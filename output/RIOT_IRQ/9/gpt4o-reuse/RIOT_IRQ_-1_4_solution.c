
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define IRQ_NUM 11

volatile int atomic_var = 0;

void irq_handler(void *arg)
{
    (void)arg;
    atomic_var = 1; // Set atomic variable to indicate interrupt occurred
    printf("Interrupt occurred\n");
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Register the interrupt handler
    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Perform some operations with interrupts disabled
    printf("Interrupts are disabled\n");
    xtimer_sleep(2);

    // Restore interrupts to their previous state
    irq_restore(state);

    // Wait for the interrupt to occur
    while (atomic_var == 0) {
        xtimer_usleep(100);
    }

    printf("Interrupt handling test completed\n");

    return 0;
}
