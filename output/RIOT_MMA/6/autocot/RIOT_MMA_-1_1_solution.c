#include "thread_flags.h"
#include "thread_arch.h"
#include "thread_config.h"
#include "thread.h"
#include "periph_conf.h"
#include "periph_conf.h"
#include "net/protnum.h"
#include "host/ble_ibeacon.h"
#include "host/ble_hs_adv.h"
#include "hashes/sha2xx_common.h"
#include "inttypes.h"
#include "host/ble_ibeacon.h"
#include "host/ble_hs_adv.h"
#include "hashes/sha2xx_common.h"
#include "inttypes.h"

#include <stdio.h>
#include "mma8x5x.h"
#include "xtimer.h"

#define SLEEP_INTERVAL 5

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    /* Add specific param initialization if needed */
};

int main(void)
{
    printf("Starting MMA8x5x reading example\n");

    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    while (1) {
        int16_t x, y, z;
        if (mma8x5x_read(&dev, &x, &y, &z) == 0) {
            printf("X: %d, Y: %d, Z: %d\n", x, y, z);
        } else {
            printf("Read failed\n");
        }

        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}