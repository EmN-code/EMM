
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
    atomic_store(&flag, 0);

    // Restore interrupts
    irq_restore(state);

    // Wait for an interrupt to occur
    while (atomic_load(&flag) == 0) {
        xtimer_usleep(1000);
    }

    printf("Interrupt handled successfully!\n");

    return 0;
}
