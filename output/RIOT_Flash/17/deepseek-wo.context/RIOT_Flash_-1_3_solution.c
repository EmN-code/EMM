
#include "nvs.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

#define NVS_NAMESPACE "storage"
#define TEMP_KEY "temperature"

int main(void)
{
    nvs_handle_t my_handle;
    int32_t temp = 0;
    int err;

    /* Initialize NVS */
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != NVS_OK) {
        printf("NVS open failed: %d\n", err);
        return 1;
    }

    /* Initialize temperature sensor (example: MMA8X5X) */
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    mma8x5x_t dev;
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("MMA8X5X initialization failed\n");
        return 1;
    }

    while (1) {
        /* Read temperature from sensor (example: MMA8X5X) */
        int16_t x, y, z;
        if (mma8x5x_read(&dev, &x, &y, &z) == MMA8X5X_OK) {
            /* Convert sensor data to temperature (example: dummy conversion) */
            temp = (x + y + z) / 3;
        }

        /* Store temperature in NVS */
        err = nvs_set_i32(my_handle, TEMP_KEY, temp);
        if (err != NVS_OK) {
            printf("NVS set failed: %d\n", err);
        } else {
            printf("Temperature updated: %d\n", temp);
        }

        /* Commit changes to NVS */
        err = nvs_commit(my_handle);
        if (err != NVS_OK) {
            printf("NVS commit failed: %d\n", err);
        }

        /* Wait for 1 second */
        xtimer_sleep(1);
    }

    /* Close NVS */
    nvs_close(my_handle);

    return 0;
}
