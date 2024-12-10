
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
    irq_set(0, (irq_handler_t)irq_handler, NULL);
    irq_enable();

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        printf("Working... %d\n", i);
        xtimer_sleep(1);
    }

    // Disable interrupts and check atomic variable
    irq_disable();
    if (atomic_var == 1) {
        printf("Interrupts were not disabled correctly\n");
    } else {
        printf("Interrupts were disabled correctly\n");
    }

    // Restore interrupts
    irq_enable();

    // Simulate more work
    for (int i = 0; i < 10; i++) {
        printf("Working... %d\n", i);
        xtimer_sleep(1);
    }

    // Final check of atomic variable
    if (atomic_var == 1) {
        printf("Interrupts were restored correctly\n");
    } else {
        printf("Interrupts were not restored correctly\n");
    }

    return 0;
}
