
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

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static volatile atomic_int interrupt_flag = ATOMIC_VAR_INIT(0);

void interrupt_handler(void)
{
    atomic_store(&interrupt_flag, 1);
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .int_pin = GPIO_PIN(0, 0)
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization failed");
        return 1;
    }

    irq_set_handler(params.int_pin, interrupt_handler);

    while (1) {
        if (atomic_load(&interrupt_flag)) {
            puts("Interrupt handled");
            atomic_store(&interrupt_flag, 0);
        }
        xtimer_sleep(1);
    }

    return 0;
}
