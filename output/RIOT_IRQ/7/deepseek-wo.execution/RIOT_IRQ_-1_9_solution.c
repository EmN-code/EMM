
#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <esp_system.h>
#include <xtimer.h>

// Define a simple atomic variable
atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Function to simulate some work
void do_work(void) {
    for (int i = 0; i < 1000000; i++) {
        // Simulate some computational work
    }
}

// Interrupt handler function
void my_interrupt_handler(void) {
    atomic_fetch_add(&atomic_var, 1); // Increment the atomic variable
}

int main(void) {
    // Disable interrupts initially
    unsigned state = irq_disable();

    // Set the interrupt handler
    irq_set_exclusive_handler(ESP32_IRQ_NUM, my_interrupt_handler);

    // Enable the interrupt
    irq_enable(ESP32_IRQ_NUM);

    // Perform some work while interrupts are enabled
    do_work();

    // Restore the interrupt state
    irq_restore(state);

    // Print the value of the atomic variable to verify interrupt handling
    printf("Atomic variable value: %d\n", atomic_load(&atomic_var));

    return 0;
}
