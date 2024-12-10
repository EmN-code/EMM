
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>
#include <atomic_utils.h>

atomic_t flag = ATOMIC_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_store(&flag, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Register the interrupt handler
    irq_set_handler(IRQ_NUM, interrupt_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Perform some critical section code
    printf("Critical section: Disabling interrupts...\n");
    xtimer_sleep(1);

    // Verify that the flag is still 0 (no interrupt occurred)
    if (atomic_load(&flag) == 0) {
        printf("Interrupts were disabled correctly.\n");
    } else {
        printf("Error: Interrupts were not disabled correctly.\n");
    }

    // Restore interrupts
    irq_restore(state);

    // Wait for an interrupt to occur
    xtimer_sleep(5);

    // Verify that the flag is now 1 (interrupt occurred)
    if (atomic_load(&flag) == 1) {
        printf("Interrupts were restored correctly.\n");
    } else {
        printf("Error: Interrupts were not restored correctly.\n");
    }

    return 0;
}
