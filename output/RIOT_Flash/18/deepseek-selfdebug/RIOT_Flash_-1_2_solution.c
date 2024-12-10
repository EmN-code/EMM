
#include "nvs.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "stdio.h"

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"
#define SLEEP_INTERVAL (1 * US_PER_SEC)

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void)
{
    nvs_handle_t my_handle;
    int err;
    uint8_t state = 0;

    // Initialize NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != NVS_OK) {
        printf("NVS open failed: %d\n", err);
        return 1;
    }

    // Initialize MMA8x5x accelerometer
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("MMA8x5x initialization failed\n");
        return 1;
    }

    while (1) {
        // Read temperature from MMA8x5x
        mma8x5x_data_t data;
        if (mma8x5x_read(&dev, &data) != MMA8X5X_OK) {
            printf("MMA8x5x read failed\n");
            continue;
        }

        // Update state based on temperature (example logic)
        if (data.x > 0) {
            state = 1;
        } else {
            state = 0;
        }

        // Write state to NVS
        err = nvs_set_u8(my_handle, NVS_KEY, state);
        if (err != NVS_OK) {
            printf("NVS set failed: %d\n", err);
            continue;
        }

        // Commit to NVS
        err = nvs_commit(my_handle);
        if (err != NVS_OK) {
            printf("NVS commit failed: %d\n", err);
            continue;
        }

        // Print current state
        printf("Device state updated to: %d\n", state);

        // Sleep for 1 second
        xtimer_sleep(SLEEP_INTERVAL);
    }

    // Close NVS
    nvs_close(my_handle);

    return 0;
}
