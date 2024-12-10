
#include <stdio.h>
#include <stdbool.h>
#include "irq.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"

volatile bool interrupt_flag = false;

void custom_interrupt_handler(void) {
    interrupt_flag = true;
}

int main(void) {
    // Initialize the MMA8x5x sensor
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_800HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization failed");
        return 1;
    }

    // Enable the interrupt on the MMA8x5x sensor
    mma8x5x_enable_int(&dev, MMA8X5X_INT_DRDY);

    // Disable interrupts globally
    unsigned state = irq_disable();

    // Set the custom interrupt handler
    mma8x5x_set_handler(&dev, custom_interrupt_handler);

    // Restore interrupts to their previous state
    irq_restore(state);

    // Main loop to check the interrupt flag
    while (1) {
        if (interrupt_flag) {
            printf("Interrupt handled!\n");
            interrupt_flag = false; // Reset the flag
        }
    }

    return 0;
}
