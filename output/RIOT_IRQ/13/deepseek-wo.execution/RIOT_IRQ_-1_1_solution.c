
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define TEST_VALUE 42

volatile atomic_int_t atomic_value = ATOMIC_INIT(TEST_VALUE);

void interrupt_handler(void)
{
    unsigned state = irq_disable();
    atomic_store(&atomic_value, atomic_load(&atomic_value) + 1);
    irq_restore(state);
}

int main(void)
{
    puts("Starting interrupt handling test...");

    // Disable interrupts initially
    unsigned state = irq_disable();

    // Set up the interrupt handler
    irq_set_handler(IRQ_NUM, interrupt_handler);

    // Enable the interrupt
    irq_enable(IRQ_NUM);

    // Restore interrupts
    irq_restore(state);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        xtimer_sleep(1);
        printf("Atomic value: %d\n", atomic_load(&atomic_value));
    }

    // Disable the interrupt again
    state = irq_disable();
    irq_disable(IRQ_NUM);
    irq_restore(state);

    // Final check of the atomic value
    printf("Final atomic value: %d\n", atomic_load(&atomic_value));

    puts("Interrupt handling test complete.");

    return 0;
}
