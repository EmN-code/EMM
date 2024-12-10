
#include <stdio.h>
#include "irq.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"

volatile int interrupt_flag = 0;

void custom_isr(void)
{
    interrupt_flag = 1;
}

int main(void)
{
    unsigned state;

    // Initialize the MMA8x5x sensor
    mma8x5x_init(I2C_DEV(0), MMA8X5X_I2C_ADDRESS);

    // Disable interrupts temporarily
    state = irq_disable();

    // Set the custom interrupt service routine
    irq_set_handler(MMA8X5X_INT_PIN, custom_isr);

    // Enable interrupts again
    irq_restore(state);

    // Enable the MMA8x5x interrupt
    mma8x5x_enable_int(MMA8X5X_INT_DRDY);

    // Main loop to check the interrupt flag
    while (1) {
        if (interrupt_flag) {
            printf("Interrupt handled successfully!\n");
            interrupt_flag = 0; // Reset the flag
        }
    }

    return 0;
}
