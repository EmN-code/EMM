
#include <stdio.h>
#include <irq.h>
#include <thread.h>
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
    irq_disable(IRQ_NUM);

    // Modify the atomic variable
    atomic_store(&flag, 0);

    // Verify the atomic variable
    if (atomic_load(&flag) == 0) {
        printf("Interrupts disabled correctly.\n");
    } else {
        printf("Interrupts were not disabled correctly.\n");
    }

    // Restore interrupts
    irq_enable(IRQ_NUM);

    // Trigger the interrupt
    irq_trigger(IRQ_NUM);

    // Verify the atomic variable
    if (atomic_load(&flag) == 1) {
        printf("Interrupts restored correctly.\n");
    } else {
        printf("Interrupts were not restored correctly.\n");
    }

    return 0;
}
