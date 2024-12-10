
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

static volatile int atomic_var = 0;

void irq_handler(void *arg)
{
    (void)arg;
    atomic_var = 1;
    printf("Interrupt handled, atomic_var set to 1\n");
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Register the interrupt handler
    irq_disable();
    irq_set(0, irq_handler, NULL);
    irq_enable();

    // Test disabling and restoring interrupts
    irq_disable();
    atomic_var = 0;
    irq_enable();

    // Simulate some work
    xtimer_sleep(1);

    // Check if the interrupt was handled
    if (atomic_var == 1) {
        printf("Interrupt was handled correctly\n");
    } else {
        printf("Interrupt was not handled\n");
    }

    return 0;
}
