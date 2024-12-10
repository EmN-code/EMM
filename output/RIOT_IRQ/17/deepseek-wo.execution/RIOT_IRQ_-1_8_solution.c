
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <esp_system.h>

// Define a simple atomic variable
atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Function to simulate some work
void do_work(void) {
    for (int i = 0; i < 1000000; i++) {
        // Simulate some computation
    }
}

// Function to test interrupt handling
void test_interrupt_handling(void) {
    // Disable interrupts
    unsigned state = irq_disable();

    // Perform some atomic operation
    atomic_fetch_add(&atomic_var, 1);

    // Simulate some work
    do_work();

    // Restore interrupts
    irq_restore(state);
}

int main(void) {
    // Initialize the system
    esp_system_init();

    // Print initial value of the atomic variable
    printf("Initial atomic_var: %d\n", atomic_load(&atomic_var));

    // Test interrupt handling
    test_interrupt_handling();

    // Print the value of the atomic variable after the test
    printf("Final atomic_var: %d\n", atomic_load(&atomic_var));

    return 0;
}
