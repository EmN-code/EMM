#include "irq.h"
#include "atomic_utils.h"

volatile uint32_t global_counter = 0;

void critical_section(void) {
    unsigned int irq_state = irq_disable(); // Disable interrupts

    // Critical section code
    global_counter++;

    irq_restore(irq_state); // Restore interrupts
}

void check_and_reset_counter(void) {
    if (atomic_load_u32(&global_counter) > THRESHOLD) {
        // Perform some action when the threshold is reached
        on_threshold_reached();
        atomic_store_u32(&global_counter, 0); // Reset the counter atomically
    }
}

int main(void) {
    // Initialize the system and any necessary components

    while (1) {
        critical_section();
        check_and_reset_counter();
        // Other application code
    }

    return 0;
}