#include <thread.h>
#include <stdio.h>
#include <kernel_defines.h>
#include <irq_arch.h>
#include <assert.h>

// Atomic variable to track interrupt state
static volatile unsigned interrupts_disabled = 0;

// Function to simulate critical section
void critical_section(void) {
    interrupts_disabled = irq_disable();
    assert(interrupts_disabled == 1);
    
    // Critical work (simulated by a simple print statement)
    printf("In critical section, interrupts disabled\n");

    irq_restore(interrupts_disabled);
    assert(interrupts_disabled == 0);
    printf("Exiting critical section, interrupts restored\n");
}

int main(void) {
    printf("Program started\n");

    // Simulate entering a critical section
    critical_section();

    printf("Program finished\n");
    return 0;
}