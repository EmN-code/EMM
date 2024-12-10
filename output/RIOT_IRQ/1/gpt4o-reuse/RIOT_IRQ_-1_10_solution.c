
#include <stdio.h>
#include <xtimer.h>
#include <irq.h>
#include <thread.h>

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

    // Attach the interrupt handler
    irq_attach(IRQ_NUM, irq_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Perform some atomic operation to verify interrupts are disabled
    atomic_var = 0;
    xtimer_sleep(1); // Sleep for 1 second
    if (atomic_var == 0) {
        printf("Interrupts are disabled correctly\n");
    } else {
        printf("Interrupts are not disabled correctly\n");
    }

    // Restore interrupts
    irq_restore(state);

    // Enable interrupts and wait for interrupt to occur
    atomic_var = 0;
    xtimer_sleep(1); // Sleep for 1 second
    if (atomic_var == 1) {
        printf("Interrupts are enabled correctly\n");
    } else {
        printf("Interrupts are not enabled correctly\n");
    }

    // Detach the interrupt handler
    irq_detach(IRQ_NUM);

    printf("Interrupt handling test completed\n");
    return 0;
}
