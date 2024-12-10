
#include "irq.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdatomic.h"

// Define a simple atomic variable
atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Function to simulate some work
void do_work(void) {
    for (int i = 0; i < 1000000; i++) {
        // Simulate work
    }
}

// Function to test interrupt handling
void test_interrupt_handling(void) {
    unsigned state = irq_disable();
    atomic_fetch_add(&atomic_var, 1);
    do_work();
    irq_restore(state);
}

int main(void) {
    printf("Starting interrupt handling test...\n");

    // Test the interrupt handling
    test_interrupt_handling();

    // Check the atomic variable to verify if interrupts were handled correctly
    if (atomic_load(&atomic_var) == 1) {
        printf("Interrupt handling test passed!\n");
    } else {
        printf("Interrupt handling test failed!\n");
    }

    return 0;
}
