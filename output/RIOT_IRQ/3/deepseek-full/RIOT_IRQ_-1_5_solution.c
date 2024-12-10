
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

#define MSG_QUEUE_SIZE 8

static msg_t msg_queue[MSG_QUEUE_SIZE];
static atomic_int interrupt_flag = ATOMIC_VAR_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_store(&interrupt_flag, 1);
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Initialize the MMA8X5X sensor
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_1_56HZ,
        .mode = MMA8X5X_MODE_NORMAL
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("MMA8X5X sensor initialization failed");
        return 1;
    }

    // Set the interrupt handler
    irq_set_handler(MMA8X5X_INT_PIN, interrupt_handler, NULL);

    // Enable the data ready interrupt
    mma8x5x_enable_int(&dev, MMA8X5X_INT_DRDY);

    while (1) {
        if (atomic_load(&interrupt_flag)) {
            puts("Interrupt received");
            atomic_store(&interrupt_flag, 0);
        }
        xtimer_sleep(1);
    }

    return 0;
}
