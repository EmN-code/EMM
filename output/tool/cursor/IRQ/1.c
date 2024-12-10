#include "irq.h"
#include "atomic_utils.h"
#include <stdio.h>

volatile uint32_t global_counter = 0;

void critical_section(void) {
    unsigned int state = irq_disable(); // Disable interrupts
    // Critical section code
    global_counter++;
    irq_restore(state); // Restore interrupts
}

void check_counter(void) {
    if (atomic_load_u32(&global_counter) > 10) {
        printf("Threshold reached!\n");
        atomic_store_u32(&global_counter, 0);
    }
}

int main(void) {
    while (1) {
        critical_section();
        check_counter();
    }
    return 0;
}