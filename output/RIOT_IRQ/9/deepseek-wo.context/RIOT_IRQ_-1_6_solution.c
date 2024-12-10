
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

    // Disable interrupts
    unsigned state = irq_disable();

    // Set up an interrupt handler
    irq_set_handler(IRQ_NUM, interrupt_handler, NULL);

    // Simulate some work
    xtimer_sleep(1);

    // Restore interrupts
    irq_restore(state);

    // Verify the flag was set by the interrupt handler
    if (atomic_load(&flag) == 1) {
        printf("Interrupt was handled correctly.\n");
    } else {
        printf("Interrupt was not handled.\n");
    }

    return 0;
}
