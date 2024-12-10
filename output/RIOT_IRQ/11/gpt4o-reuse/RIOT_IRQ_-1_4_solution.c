
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define IRQ_LINE 11

volatile int atomic_var = 0;

void irq_handler(void *arg)
{
    (void)arg;
    atomic_var = 1;
    printf("Interrupt handled, atomic_var set to 1\n");
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Disable interrupts
    unsigned state = irq_disable();

    // Set the interrupt handler
    irq_set_handler(IRQ_LINE, irq_handler, NULL);

    // Enable interrupts
    irq_restore(state);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        xtimer_sleep(1);
        printf("Working... %d\n", i);
    }

    // Disable interrupts again to check if atomic_var is still 0
    state = irq_disable();
    if (atomic_var == 0) {
        printf("Interrupts were disabled correctly\n");
    } else {
        printf("Interrupts were not disabled correctly\n");
    }
    irq_restore(state);

    return 0;
}
