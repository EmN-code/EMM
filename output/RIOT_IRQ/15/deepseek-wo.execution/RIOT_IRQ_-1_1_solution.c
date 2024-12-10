
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
    printf("Starting interrupt handling test...\n");

    // Initialize the atomic variable
    atomic_store(&test_atomic, TEST_VALUE);

    // Disable interrupts and verify the atomic variable
    unsigned state = irq_disable();
    if (atomic_load(&test_atomic) == TEST_VALUE) {
        printf("Interrupts disabled successfully.\n");
    } else {
        printf("Error: Interrupts were not disabled correctly.\n");
        return 1;
    }

    // Simulate an interrupt by calling the handler manually
    interrupt_handler();

    // Restore interrupts and verify the atomic variable
    irq_restore(state);
    if (atomic_load(&test_atomic) == TEST_VALUE + 1) {
        printf("Interrupts restored successfully.\n");
    } else {
        printf("Error: Interrupts were not restored correctly.\n");
        return 1;
    }

    printf("Interrupt handling test completed successfully.\n");
    return 0;
}
