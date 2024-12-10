#include "irq.h"
#include "atomic_utils.h"

static atomic_t my_atomic_var = ATOMIC_INIT(0);

void my_interrupt_handler(void *arg) {
    (void)arg; // Unused parameter
    atomic_add(&my_atomic_var, 1);
}

void setup_interrupts(void) {
    unsigned int state = irq_disable(); // Disable interrupts

    // Configure your interrupt source here
    // For example, setting up a timer interrupt or another peripheral

    irq_restore(state); // Restore the previous interrupt state
}

int main(void) {
    setup_interrupts();

    // Simulate some operations
    for (int i = 0; i < 10; i++) {
        irq_disable();
        // Critical section
        atomic_add(&my_atomic_var, 1);
        irq_enable();
    }

    // Check the value of the atomic variable
    if (atomic_load(&my_atomic_var) == expected_value) {
        // Interrupt handling works correctly
    }

    return 0;
}