
#include "irq.h"
#include "stddef.h"
#include "timex.h"
#include "xtimer.h"
#include "mma8x5x.h"
#include "stdlib.h"
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

static volatile atomic_int interrupt_count = ATOMIC_VAR_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_fetch_add(&interrupt_count, 1);
}

int main(void)
{
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = MMA8X5X_I2C_DEV,
        .addr = MMA8X5X_ADDR_DEFAULT,
        .mode = MMA8X5X_MODE_NORMAL,
        .rate = MMA8X5X_RATE_1_56HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization failed");
        return 1;
    }

    mma8x5x_enable_int(&dev, MMA8X5X_INT_DRDY, interrupt_handler, NULL);

    while (1) {
        unsigned state = irq_disable();
        int count = atomic_load(&interrupt_count);
        irq_restore(state);

        printf("Interrupt count: %d\n", count);
        xtimer_sleep(1);
    }

    return 0;
}
