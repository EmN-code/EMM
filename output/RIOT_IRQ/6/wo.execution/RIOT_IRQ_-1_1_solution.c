#include <irq_arch.h>
#include <stdbool.h>
#include <stdatomic.h>

atomic_bool interrupt_status;

void disable_and_restore_irq(void) {
    // Disable interrupts
    unsigned state = irq_disable();
    
    // Simulate critical section by modifying atomic variable
    atomic_store(&interrupt_status, true);
    
    // Restore interrupts
    irq_restore(state);
}

int main(void) {
    // Initial interrupt status
    atomic_store(&interrupt_status, false);

    // Perform the disable and restore sequence
    disable_and_restore_irq();

    // Check the status; should be true if interrupt handling was correctly temporary disabled
    if (atomic_load(&interrupt_status)) {
        // Interrupts were successfully handled
    } else {
        // Something went wrong in interrupt handling
    }

    return 0;
}
