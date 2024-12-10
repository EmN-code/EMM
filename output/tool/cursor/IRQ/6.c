#include "irq.h"
#include "atomic_utils.h"

volatile uint32_t global_counter = 0;

void critical_section_example(void) {
    unsigned int irq_state = irq_disable(); // Disable interrupts

    // Perform atomic operations
    atomic_add_u32(&global_counter, 1);

    irq_restore(irq_state); // Restore interrupts
}

void check_and_handle_threshold(void) {
    if (atomic_load_u32(&global_counter) > THRESHOLD) {
        // Handle threshold reached
        on_threshold_reached();
        atomic_store_u32(&global_counter, 0);
    }
}

int main(void) {
    // Initialize system and peripherals

    while (1) {
        critical_section_example();
        check_and_handle_threshold();
        // Other application logic
    }

    return 0;
}