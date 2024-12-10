#include <xtimer.h>    // timer functions
#include <stdatomic.h>  // for atomic operations
#include <irq.h>        // for irq functions
#include <stdio.h>      // for printing to console

atomic_int interrupt_counter = 0;

void test_interrupt_handling(void) {
    unsigned state;

    // Disable interrupts
    state = irq_disable();
    printf("Interrupts disabled\n");

    // Simulated computation while interrupts are disabled
    atomic_fetch_add(&interrupt_counter, 1);

    // Restore interrupts
    irq_restore(state);
    printf("Interrupts restored\n");

    // Verify correct count
    int count = atomic_load(&interrupt_counter);
    printf("Interrupt counter value: %d\n", count);
}

int main(void) {
    while (1) {
        // Run the interrupt handling test
        test_interrupt_handling();

        // Sleep for a while before re-testing
        xtimer_sleep(2);
    }
    return 0;
}