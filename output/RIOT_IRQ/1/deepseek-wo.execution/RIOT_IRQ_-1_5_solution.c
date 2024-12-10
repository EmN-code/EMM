
#include "irq.h"
#include "stddef.h"
#include "timex.h"
#include "xtimer.h"
#include "mma8x5x.h"
#include "stdlib.h"
#include "arpa/inet.h"
#include "mma8x5x_regs.h"
#include "evtimer_msg.h"
#include "kernel_defines.h"
#include "msg.h"
#include "ztimer.h"
#include "time_units.h"
#include "evtimer_mbox.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "stdio.h"

static atomic_int atomic_var = ATOMIC_VAR_INIT(0);

void interrupt_handler(void)
{
    atomic_fetch_add(&atomic_var, 1);
}

int main(void)
{
    unsigned state;

    // Initialize the MMA8X5X sensor
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = MMA8X5X_I2C_DEV,
        .addr = MMA8X5X_ADDR_DEFAULT,
        .rate = MMA8X5X_RATE_1_56HZ,
        .mode = MMA8X5X_MODE_NORMAL
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization of MMA8X5X sensor failed");
        return 1;
    }

    // Enable the data ready interrupt
    mma8x5x_enable_int(&dev, MMA8X5X_INT_DRDY, interrupt_handler);

    // Test interrupt disable and restore
    state = irq_disable();
    printf("Interrupt disabled, atomic_var: %d\n", atomic_load(&atomic_var));
    xtimer_sleep(2);
    printf("After 2 seconds, atomic_var: %d\n", atomic_load(&atomic_var));
    irq_restore(state);

    xtimer_sleep(2);
    printf("After restoring interrupts, atomic_var: %d\n", atomic_load(&atomic_var));

    return 0;
}
