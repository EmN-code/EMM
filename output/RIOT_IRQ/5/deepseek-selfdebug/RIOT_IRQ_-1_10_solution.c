
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

    // Register an interrupt handler
    irq_set_handler(IRQ_NUM, interrupt_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Perform some critical section code
    printf("Critical section: Interrupts are disabled\n");
    xtimer_sleep(2);

    // Verify that the interrupt handler did not run
    if (atomic_load(&flag) == 1) {
        printf("Error: Interrupt handler ran while interrupts were disabled\n");
    } else {
        printf("Interrupts were successfully disabled\n");
    }

    // Restore interrupts
    irq_restore(state);

    // Wait for the interrupt to be handled
    xtimer_sleep(2);

    // Verify that the interrupt handler ran
    if (atomic_load(&flag) == 1) {
        printf("Interrupt handler ran successfully\n");
    } else {
        printf("Error: Interrupt handler did not run\n");
    }

    return 0;
}
