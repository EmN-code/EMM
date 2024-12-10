
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <timex.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <kernel_defines.h>
#include <time_units.h>
#include <msg.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <xtimer.h>
#include <arpa/inet.h>
#include <irq.h>
#include <ztimer.h>
#include <stddef.h>

// Define a structure to hold the device and parameters
typedef struct {
    mma8x5x_t dev;
    mma8x5x_params_t params;
} mma8x5x_device_t;

// Global atomic variable to track interrupt state
atomic_int interrupt_flag = ATOMIC_VAR_INIT(0);

// Interrupt handler function
void interrupt_handler(void)
{
    atomic_store(&interrupt_flag, 1);
}

int main(void)
{
    // Initialize the device structure
    mma8x5x_device_t mma8x5x_dev;

    // Set up the parameters for the MMA8X5X sensor
    mma8x5x_dev.params.i2c = I2C_DEV(0);
    mma8x5x_dev.params.addr = MMA8X5X_I2C_ADDRESS;
    mma8x5x_dev.params.mode = MMA8X5X_MODE_NORMAL;
    mma8x5x_dev.params.int_pin = MMA8X5X_INT_PIN;

    // Initialize the MMA8X5X sensor
    int res = mma8x5x_init(&mma8x5x_dev.dev, &mma8x5x_dev.params);
    if (res != MMA8X5X_OK) {
        printf("Failed to initialize MMA8X5X sensor\n");
        return 1;
    }

    // Set the interrupt handler
    irq_set_handler(mma8x5x_dev.params.int_pin, interrupt_handler);

    // Main loop to check the interrupt flag
    while (1) {
        if (atomic_load(&interrupt_flag) == 1) {
            printf("Interrupt received!\n");
            atomic_store(&interrupt_flag, 0); // Reset the flag
        }
        xtimer_sleep(1); // Sleep for 1 second
    }

    return 0;
}
