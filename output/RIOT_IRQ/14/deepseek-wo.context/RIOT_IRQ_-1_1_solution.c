
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define TEST_VALUE 42

volatile atomic_int_t atomic_value = ATOMIC_INIT(TEST_VALUE);

void interrupt_handler(void)
{
    unsigned state = irq_disable();
    atomic_store(&atomic_value, TEST_VALUE + 1);
    irq_restore(state);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Disable interrupts initially
    unsigned state = irq_disable();

    // Set up the interrupt handler
    irq_set_handler(IRQ_NUM, interrupt_handler);

    // Enable the interrupt
    irq_enable(IRQ_NUM);

    // Restore interrupts
    irq_restore(state);

    // Simulate some work
    xtimer_sleep(1);

    // Check the atomic value
    int value = atomic_load(&atomic_value);
    if (value == TEST_VALUE + 1) {
        printf("Interrupt handling and atomic operations worked correctly.\n");
    } else {
        printf("Interrupt handling or atomic operations failed.\n");
    }

    return 0;
}
