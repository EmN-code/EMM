#include "irq.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

volatile uint32_t global_counter = 0;

void critical_section(void) {
    unsigned int irq_state = irq_disable(); // Disable interrupts

    // Critical section: perform operations on global_counter
    global_counter++;

    irq_restore(irq_state); // Restore interrupts
}

void check_and_reset_counter(void) {
    if (global_counter > 10) {
        printf("Threshold reached: %u\n", global_counter);
        global_counter = 0;
    }
}

int main(void) {
    while (1) {
        critical_section();
        check_and_reset_counter();
    }
    return 0;
}