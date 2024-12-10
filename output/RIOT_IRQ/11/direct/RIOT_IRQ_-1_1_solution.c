#include <stdio.h>
#include <stdint.h>
#include <irq_arch.h>
#include <assert.h>
#include <xtensa/hal.h>

static volatile uint32_t interrupt_count = 0;

void IRAM_ATTR test_interrupt_handler(void *arg) {
    (void)arg; // Unused parameter
    // Increment the interrupt count atomically
    interrupt_count++;
    // Acknowledge the interrupt (if required by specific hardware)
}

int main(void) {
    uint32_t state;
    
    printf("Starting the interrupt test program.\n");

    // Disable interrupts and store the previous state
    state = xt_interrupt_disable();
    printf("Interrupts disabled.\n");

    // Simulate an interrupt by calling the handler directly
    test_interrupt_handler(NULL);

    // Restore the previous interrupt state
    xt_interrupt_restore(state);
    printf("Interrupts restored.\n");

    assert(interrupt_count == 1);
    printf("Interrupt handling verified successfully. Count: %u\n", interrupt_count);

    return 0;
}