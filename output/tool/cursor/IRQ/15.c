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
    if (atomic_load_u32(&global_counter) > 10) {
        // Perform some action
        atomic_store_u32(&global_counter, 0);
    }
}

int main(void) {
    while (1) {
        critical_section();
        check_and_reset_counter();
    }
    return 0;
}