
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define TEST_VALUE 42

volatile atomic_t test_atomic = ATOMIC_INIT(TEST_VALUE);

void interrupt_handler(void)
{
    unsigned state = irq_disable();
    atomic_store(&test_atomic, TEST_VALUE + 1);
    irq_restore(state);
}

int main(void)
{
    unsigned state = irq_disable();
    atomic_store(&test_atomic, TEST_VALUE);
    irq_restore(state);

    // Simulate an interrupt
    interrupt_handler();

    // Verify the atomic variable was updated correctly
    if (atomic_load(&test_atomic) == (TEST_VALUE + 1)) {
        printf("Interrupt handling and atomic variable update successful!\n");
    } else {
        printf("Interrupt handling or atomic variable update failed!\n");
    }

    return 0;
}
