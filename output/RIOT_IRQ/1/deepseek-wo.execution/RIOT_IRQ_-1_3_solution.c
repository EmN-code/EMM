
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include "mma8x5x.h"
#include "mma8x5x_regs.h"

// Define a simple atomic variable
atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Function to simulate some work
void do_work(void) {
    for (int i = 0; i < 1000000; i++) {
        // Simulate some computation
    }
}

// Interrupt handler function
void interrupt_handler(void) {
    // Increment the atomic variable
    atomic_fetch_add(&atomic_var, 1);
}

int main(void) {
    // Initialize the MMA8X5X sensor (assuming it's connected to the ESP32)
    mma8x5x_t dev;
    if (mma8x5x_init(&dev, MMA8X5X_I2C_DEV(0), MMA8X5X_ADDR_DEFAULT, MMA8X5X_TYPE_MMA8452) != MMA8X5X_OK) {
        puts("Initialization of MMA8X5X sensor failed");
        return 1;
    }

    // Enable the interrupt (assuming the MMA8X5X sensor can generate interrupts)
    // This is a hypothetical setup since we are not using GPIO interrupts
    mma8x5x_enable_int(&dev, MMA8X5X_INT_DRDY);

    // Attach the interrupt handler (hypothetical, as we are not using GPIO interrupts)
    // This is a placeholder for attaching an interrupt handler to a specific interrupt source
    // In a real scenario, you would attach this to a specific interrupt source, not just a function call
    // For example, if using a timer interrupt, you would attach the handler to the timer interrupt source
    // Here, we simulate the interrupt handler being called
    interrupt_handler();

    // Perform some work while interrupts are enabled
    do_work();

    // Check the atomic variable to verify if the interrupt was handled correctly
    int final_value = atomic_load(&atomic_var);
    printf("Atomic variable value after interrupt: %d\n", final_value);

    return 0;
}
