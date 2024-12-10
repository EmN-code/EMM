
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_t flag = ATOMIC_INIT(0);

void irq_handler(void *arg)
{
    (void)arg;
    atomic_store(&flag, 1);
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Set the interrupt handler
    irq_set_handler(0, irq_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Trigger the interrupt
    irq_trigger(0);

    // Restore interrupts
    irq_restore(state);

    // Check the atomic flag
    if (atomic_load(&flag) == 1) {
        printf("Interrupt was handled correctly\n");
    } else {
        printf("Interrupt was not handled\n");
    }

    return 0;
}
