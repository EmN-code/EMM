#include <thread.h>
#include <stdio.h>
#include "mma8x5x.h"
#include "periph_conf.h"
#include "xtimer.h"

#define SLEEP_INTERVAL (5U * US_PER_SEC)

static mma8x5x_t dev;

static void *mma8x5x_reader_thread(void *arg) {
    (void)arg;
    while (1) {
        /* Check if the device is ready */
        if (mma8x5x_is_ready(&dev)) {
            /* Read sensor values */
            int16_t x, y, z;
            mma8x5x_read(&dev, &x, &y, &z);
            
            /* Print the readings */
            printf("MMA8x5x readings - X: %d, Y: %d, Z: %d\n", x, y, z);
        } else {
            printf("MMA8x5x not ready\n");
        }
        
        /* Sleep for the defined interval */
        xtimer_sleep(SLEEP_INTERVAL / US_PER_SEC);
    }
    return NULL;
}

int main(void) {
    /* Initialize the MMA8x5x sensor */
    mma8x5x_params_t params;
    params.i2c = I2C_DEV(0);  // Use I2C device 0
    params.addr = 0x1D;       // Default I2C address for MMA8x5x
    mma8x5x_init(&dev, &params);

    /* Create a thread to periodically read the sensor */
    static char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mma8x5x_reader_thread,
                  NULL, "mma8x5x_reader");

    /* The main thread has nothing more to do, let it sleep */
    while (1) {
        thread_sleep();
    }

    return 0;
}