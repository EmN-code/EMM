
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>

// Define a simple atomic variable
atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Function to simulate some work
void do_work(void) {
    for (int i = 0; i < 1000000; i++) {
        // Simulate work
    }
}

// Interrupt handler function
void irq_handler(void) {
    atomic_fetch_add(&atomic_var, 1); // Increment the atomic variable
}

int main(void) {
    // Enable interrupts
    unsigned state = irq_enable();

    // Simulate some work
    do_work();

    // Restore the interrupt state
    irq_restore(state);

    // Print the value of the atomic variable
    printf("Atomic variable value: %d\n", atomic_load(&atomic_var));

    return 0;
}
