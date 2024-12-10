
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int atomic_var = ATOMIC_INIT(0);

void irq_handler(void *arg)
{
    (void)arg;
    atomic_store(&atomic_var, 1);
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Set the interrupt handler
    irq_set_handler(0, irq_handler, NULL);

    // Enable interrupts
    irq_enable();

    // Simulate some work
    for (int i = 0; i < 1000000; i++) {
        // Busy loop
    }

    // Disable interrupts
    unsigned state = irq_disable();

    // Check the atomic variable
    if (atomic_load(&atomic_var) == 1) {
        printf("Interrupts were disabled and restored correctly\n");
    } else {
        printf("Interrupts were not disabled and restored correctly\n");
    }

    // Restore interrupts
    irq_restore(state);

    return 0;
}
