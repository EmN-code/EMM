#include "mma8x5x_regs.h"
#include "irq_arch.h"
#include "irq.h"
#include "architecture.h"
#include <stdio.h>
#include <stdatomic.h>

// This simple example demonstrates the enabling and restoring of interrupts.
atomic_int interrupt_counter = 0;

void test_interrupt_handling(void) {
    unsigned state;

    // Simulate enabling interrupts
    state = irq_enable();

    // Do some operation that requires interrupts to be enabled
    atomic_fetch_add(&interrupt_counter, 1);
    printf("Interrupts enabled, counter incremented: %d\n", interrupt_counter);

    // Simulate restoring interrupts
    irq_restore(state);
}

int main(void) {
    printf("Starting interrupt handling test...\n");
    test_interrupt_handling();
    printf("Test completed. Final counter: %d\n", interrupt_counter);
    return 0;
}