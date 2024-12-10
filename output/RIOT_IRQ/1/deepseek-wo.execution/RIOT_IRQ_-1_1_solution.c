
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
    puts("Starting interrupt handling test...");

    // Initialize the atomic variable
    atomic_store(&test_atomic, TEST_VALUE);

    // Disable interrupts and verify the atomic variable
    unsigned state = irq_disable();
    if (atomic_load(&test_atomic) == TEST_VALUE) {
        puts("Interrupts disabled, atomic variable unchanged.");
    } else {
        puts("Error: Atomic variable changed while interrupts were disabled.");
    }
    irq_restore(state);

    // Trigger the interrupt handler
    interrupt_handler();

    // Verify the atomic variable after interrupt
    if (atomic_load(&test_atomic) == TEST_VALUE + 1) {
        puts("Interrupt handled correctly, atomic variable updated.");
    } else {
        puts("Error: Atomic variable not updated correctly.");
    }

    puts("Interrupt handling test completed.");

    return 0;
}
