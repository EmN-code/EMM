
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>
#include <atomic_utils.h>

atomic_int counter = ATOMIC_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_add(&counter, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Register the interrupt handler
    irq_set_handler(IRQ_NUM, interrupt_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Perform some operations while interrupts are disabled
    for (int i = 0; i < 1000000; i++) {
        atomic_add(&counter, 1);
    }

    // Restore interrupts to their previous state
    irq_restore(state);

    // Wait for a while to let interrupts happen
    xtimer_sleep(5);

    // Print the final value of the counter
    printf("Counter value: %d\n", atomic_get(&counter));

    return 0;
}
