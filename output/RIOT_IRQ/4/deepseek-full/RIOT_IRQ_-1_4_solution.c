
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
#include "time_units.h"
#include "mma8x5x.h"

#define MSG_QUEUE_SIZE 10

static msg_t msg_queue[MSG_QUEUE_SIZE];
static atomic_bool interrupt_handled = ATOMIC_VAR_INIT(false);

void custom_interrupt_handler(void)
{
    atomic_store(&interrupt_handled, true);
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Initialize the MMA8x5x sensor
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .mode = MMA8X5X_MODE_NORMAL,
        .rate = MMA8X5X_RATE_100HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization of MMA8x5x sensor failed");
        return 1;
    }

    // Disable interrupts
    unsigned state = irq_disable();

    // Set custom interrupt handler
    irq_set_handler(MMA8X5X_INT_PIN, custom_interrupt_handler);

    // Enable interrupts
    irq_restore(state);

    while (1) {
        // Simulate some work
        xtimer_sleep(1);

        // Check if interrupt was handled
        if (atomic_load(&interrupt_handled)) {
            puts("Interrupt handled successfully");
            atomic_store(&interrupt_handled, false);
        }
    }

    return 0;
}
