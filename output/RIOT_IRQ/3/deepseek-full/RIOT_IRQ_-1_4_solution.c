
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <timex.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <xtimer.h>
#include <irq.h>
#include <msg.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <ztimer.h>
#include <stddef.h>
#include <time_units.h>

#define MSG_QUEUE_SIZE (8)

static msg_t _msg_queue[MSG_QUEUE_SIZE];
static atomic_int interrupt_flag = ATOMIC_VAR_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_store(&interrupt_flag, 1);
}

int main(void)
{
    msg_init_queue(_msg_queue, MSG_QUEUE_SIZE);

    // Initialize the MMA8X5X sensor
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .mode = MMA8X5X_MODE_NORMAL,
        .rate = MMA8X5X_RATE_1_56HZ
    };
    mma8x5x_init(&dev, &params);

    // Set the interrupt handler
    irq_set_handler(MMA8X5X_INT_PIN, interrupt_handler, NULL);

    // Enable the interrupt
    mma8x5x_enable_int(&dev, MMA8X5X_INT_DRDY);

    while (1) {
        // Simulate some work
        xtimer_sleep(1);

        // Disable interrupts
        unsigned state = irq_disable();

        // Check the interrupt flag
        if (atomic_load(&interrupt_flag) == 1) {
            printf("Interrupt handled correctly.\n");
            atomic_store(&interrupt_flag, 0);
        }

        // Restore interrupts
        irq_restore(state);
    }

    return 0;
}
