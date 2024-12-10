#include "thread_flags.h"
#include "thread_arch.h"
#include "thread.h"
#include "periph_conf.h"
#include "net/protnum.h"
#include "inttypes.h"
#include "host/ble_ibeacon.h"
#include "hashes/sha2xx_common.h"

#include <stdio.h>
#include "ztimer.h"
#include "mma8x5x.h"

#define INTERVAL (5U * US_PER_SEC)   // 5 seconds interval

static mma8x5x_t dev;

void *mma8x5x_handler(void *arg) {
    (void)arg;
    mma8x5x_data_t data;

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);

        if (mma8x5x_read(&dev, &data) == 0) {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        } else {
            printf("Failed to read ACC sensor\n");
        }
    }

    return NULL;
}

int main(void) {
    mma8x5x_params_t params = {
        .params = { /* Initialize with specific parameters as required */ }
    };

    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Failed to initialize the MMA8x5x sensor\n");
        return 1;
    }

    // Start the thread to handle MMA8x5x readings
    thread_create((char *)malloc(THREAD_STACKSIZE_DEFAULT),
                  THREAD_STACKSIZE_DEFAULT,
                  THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST,
                  mma8x5x_handler,
                  NULL, "mma8x5x_thread");

    return 0;
}