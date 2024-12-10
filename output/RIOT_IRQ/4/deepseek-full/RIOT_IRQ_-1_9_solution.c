
#include "msg.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "irq.h"
#include "stdlib.h"
#include "ztimer.h"
#include "stddef.h"
#include "net/sock/util.h"
#include "mutex.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "timex.h"
#include "net/emcute.h"
#include "mma8x5x_regs.h"
#include "stdio.h"
#include "net/gcoap.h"
#include "kernel_defines.h"
#include "time_units.h"
#include "net/sock/udp.h"
#include "mma8x5x.h"

atomic_int interrupt_flag = ATOMIC_VAR_INIT(0);

void interrupt_handler(void)
{
    atomic_store(&interrupt_flag, 1);
}

int main(void)
{
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    // Initialize the MMA8X5X sensor
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_DEFAULT_ADDRESS,
        .mode = MMA8X5X_MODE_NORMAL,
        .rate = MMA8X5X_RATE_1_56HZ
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("MMA8X5X initialization failed");
        return 1;
    }

    // Set the interrupt handler
    irq_set_handler(dev.params.int_pin, interrupt_handler);

    while (1) {
        if (atomic_load(&interrupt_flag)) {
            puts("Interrupt received");
            atomic_store(&interrupt_flag, 0);
        }
        xtimer_sleep(1);
    }

    return 0;
}
