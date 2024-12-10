
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
    unsigned state = irq_disable();
    atomic_store(&atomic_value, TEST_VALUE);
    irq_restore(state);

    // Simulate an interrupt
    interrupt_handler();

    // Verify the value after interrupt
    int value = atomic_load(&atomic_value);
    if (value == TEST_VALUE + 1) {
        printf("Interrupt handling and atomic operations worked correctly.\n");
    } else {
        printf("Interrupt handling or atomic operations failed.\n");
    }

    return 0;
}
