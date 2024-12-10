
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <xtimer.h>
#include "mma8x5x.h"
#include "periph/i2c.h"
#include "periph/nvs.h"

#define NVS_NAMESPACE "device_state"
#define NVS_KEY "state"

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void)
{
    int32_t state = 0;
    nvs_handle_t my_handle;
    int err;

    /* Initialize the MMA8x5x accelerometer */
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize MMA8x5x accelerometer");
        return 1;
    }

    /* Open NVS namespace */
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != 0) {
        puts("Failed to open NVS namespace");
        return 1;
    }

    while (1) {
        mma8x5x_data_t data;
        mma8x5x_read(&dev, &data);

        /* Simulate temperature update */
        state = (state == 0) ? 1 : 0;

        /* Store the state in NVS */
        err = nvs_set(my_handle, NVS_KEY, &state, sizeof(state));
        if (err != 0) {
            puts("Failed to write to NVS");
            return 1;
        }

        /* Commit the changes to NVS */
        err = nvs_commit(my_handle);
        if (err != 0) {
            puts("Failed to commit to NVS");
            return 1;
        }

        printf("State updated to: %ld\n", (long int)state);

        /* Wait for 1 second */
        xtimer_sleep(1);
    }

    /* Close NVS handle */
    nvs_close(my_handle);

    return 0;
}
