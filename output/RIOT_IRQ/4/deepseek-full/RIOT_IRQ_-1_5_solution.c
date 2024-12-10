
#include "msg.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "irq.h"
#include "stdlib.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "timex.h"
#include "mma8x5x_regs.h"
#include "stdio.h"
#include "kernel_defines.h"
#include "time_units.h"
#include "mma8x5x.h"

#define MMA8X5X_I2C_ADDRESS CONFIG_MMA8X5X_I2C_ADDRESS
#define MMA8X5X_MODE_NORMAL MMA8X5X_MODE_NORMAL
#define MMA8X5X_INT_PIN MMA8X5X_INT_PIN

static mma8x5x_t dev;
static atomic_int interrupt_count = ATOMIC_VAR_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_fetch_add(&interrupt_count, 1);
}

int main(void)
{
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .mode = MMA8X5X_MODE_NORMAL
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization failed");
        return 1;
    }

    puts("Initialization successful");

    // Enable interrupts
    irq_enable();

    // Set interrupt handler
    irq_set_handler(MMA8X5X_INT_PIN, interrupt_handler, NULL);

    while (1) {
        xtimer_sleep(1);
        int current_count = atomic_load(&interrupt_count);
        printf("Interrupt count: %d\n", current_count);
    }

    return 0;
}
