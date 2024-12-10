
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

    // Initialize the interrupt handler
    irq_disable();
    irq_set_handler(0, irq_handler, NULL);
    irq_enable();

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        printf("Working... %d\n", i);
        xtimer_sleep(1);
    }

    // Disable interrupts and check atomic variable
    irq_disable();
    if (atomic_var == 1) {
        printf("Interrupts were enabled during work, atomic_var is 1\n");
    } else {
        printf("Interrupts were disabled during work, atomic_var is 0\n");
    }
    irq_enable();

    printf("End of interrupt handling test\n");
    return 0;
}
