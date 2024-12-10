
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

MODULE_LICENSE("GPL");

static volatile int atomic_var = 0;

static void irq_handler(void *arg)
{
    (void)arg;
    atomic_var = 1;
    printf("Interrupt handled, atomic_var set to 1\n");
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Register the interrupt handler
    irq_set(11, irq_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Modify the atomic variable
    atomic_var = 0;

    // Restore interrupts
    irq_restore(state);

    // Wait for the interrupt to be handled
    xtimer_sleep(1);

    // Check if the interrupt was handled
    if (atomic_var == 1) {
        printf("Interrupt handling verified, atomic_var is 1\n");
    } else {
        printf("Interrupt handling failed, atomic_var is 0\n");
    }

    return 0;
}
