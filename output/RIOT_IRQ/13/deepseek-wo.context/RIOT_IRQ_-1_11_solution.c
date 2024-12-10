
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>
#include <atomic_utils.h> // Include the correct header for atomic operations

volatile atomic_t interrupt_flag = ATOMIC_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_store(&interrupt_flag, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Disable interrupts
    unsigned state = irq_disable();

    // Set up an interrupt handler
    irq_set_handler(IRQ_NUM, interrupt_handler, NULL);

    // Simulate an interrupt by manually triggering the handler
    interrupt_handler(NULL);

    // Verify the interrupt flag was set
    if (atomic_load(&interrupt_flag) == 1) {
        printf("Interrupt flag set correctly.\n");
    } else {
        printf("Interrupt flag not set.\n");
    }

    // Restore interrupts
    irq_restore(state);

    printf("Interrupt handling test completed.\n");

    return 0;
}
