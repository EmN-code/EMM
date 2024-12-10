
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>
#include <atomic_utils.h>

atomic_t counter = ATOMIC_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_add(&counter, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Register an interrupt handler
    irq_set_handler(0, interrupt_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Perform some operations while interrupts are disabled
    for (int i = 0; i < 1000000; i++) {
        atomic_add(&counter, 1);
    }

    // Restore interrupts
    irq_restore(state);

    // Wait for a while to let the interrupt handler run
    xtimer_sleep(1);

    // Check the counter value
    if (atomic_get(&counter) > 1000000) {
        printf("Interrupts were correctly disabled and restored.\n");
    } else {
        printf("Interrupts were not correctly disabled and restored.\n");
    }

    return 0;
}
