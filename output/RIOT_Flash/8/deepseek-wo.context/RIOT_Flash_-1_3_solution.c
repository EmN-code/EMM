
#include "nvs.h"
#include "xtimer.h"
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

    /* Initialize MMA8x5x accelerometer */
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    mma8x5x_t dev;
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("MMA8x5x initialization failed\n");
        return 1;
    }

    while (1) {
        /* Read temperature from MMA8x5x */
        mma8x5x_data_t data;
        mma8x5x_read(&dev, &data);
        temp = (data.x + data.y + data.z) / 3; // Dummy temperature calculation

        /* Store temperature in NVS */
        err = nvs_set_i32(my_handle, TEMP_KEY, temp);
        if (err != NVS_OK) {
            printf("NVS set failed: %d\n", err);
        } else {
            printf("Temperature updated: %d\n", temp);
        }

        /* Commit changes to NVS */
        nvs_commit(my_handle);

        /* Wait for 1 second */
        xtimer_sleep(1);
    }

    /* Close NVS */
    nvs_close(my_handle);

    return 0;
}
