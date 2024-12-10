
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define TEST_VALUE 42

volatile atomic_int_t atomic_var = ATOMIC_INIT(TEST_VALUE);

void interrupt_handler(void)
{
    unsigned state = irq_disable();
    atomic_store(&atomic_var, atomic_load(&atomic_var) + 1);
    irq_restore(state);
}

int main(void)
{
    puts("Starting interrupt handling test...");

    // Disable interrupts initially
    unsigned state = irq_disable();

    // Set up the interrupt handler
    irq_set_handler(0, interrupt_handler);

    // Enable interrupts
    irq_restore(state);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        xtimer_sleep(1);
        printf("Atomic variable value: %d\n", atomic_load(&atomic_var));
    }

    // Verify the atomic variable value
    if (atomic_load(&atomic_var) == TEST_VALUE + 10) {
        puts("Interrupt handling test passed!");
    } else {
        puts("Interrupt handling test failed!");
    }

    return 0;
}
