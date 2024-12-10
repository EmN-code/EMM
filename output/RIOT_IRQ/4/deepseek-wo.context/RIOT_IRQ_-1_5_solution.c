
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
    irq_disable();
    irq_set_handler(0, interrupt_handler, NULL);
    irq_enable();

    // Simulate some work
    xtimer_sleep(1);

    // Disable interrupts and check the flag
    irq_disable();
    if (atomic_load(&flag) == 1) {
        printf("Error: Interrupt occurred while interrupts were disabled.\n");
    } else {
        printf("Interrupts disabled successfully.\n");
    }

    // Restore interrupts and check the flag again
    irq_enable();
    xtimer_sleep(1);

    if (atomic_load(&flag) == 1) {
        printf("Interrupts restored successfully.\n");
    } else {
        printf("Error: Interrupt did not occur after restoring interrupts.\n");
    }

    return 0;
}
