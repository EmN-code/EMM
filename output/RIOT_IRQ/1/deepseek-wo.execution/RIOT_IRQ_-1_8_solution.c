
#include <net/emcute.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <mma8x5x.h>
#include <kernel_defines.h>
#include <timex.h>
#include <msg.h>
#include <ztimer.h>
#include <mutex.h>
#include <xtimer.h>
#include <stddef.h>
#include <evtimer_msg.h>
#include <stdatomic.h>
#include <mma8x5x_regs.h>
#include <net/sock/udp.h>
#include <time_units.h>
#include <stdbool.h>
#include <net/sock/util.h>
#include <stdio.h>
#include <net/gcoap.h>
#include <irq.h>
#include <evtimer_mbox.h>

volatile atomic_int interrupt_flag = ATOMIC_VAR_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_store(&interrupt_flag, 1);
}

int main(void)
{
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_DEFAULT_ADDRESS,
        .mode = MMA8X5X_MODE_NORMAL,
        .int_pin = GPIO_PIN(0, 0)  // Corrected to use GPIO_PIN
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization failed");
        return 1;
    }

    // Set the interrupt handler
    irq_set_handler(params.int_pin, interrupt_handler, NULL);

    while (1) {
        if (atomic_load(&interrupt_flag)) {
            puts("Interrupt detected!");
            atomic_store(&interrupt_flag, 0);
        }
        xtimer_sleep(1);
    }

    return 0;
}
