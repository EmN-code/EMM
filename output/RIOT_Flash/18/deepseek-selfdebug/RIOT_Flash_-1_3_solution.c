
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "nvs.h"

#define MMA8X5X_I2C_ADDRESS CONFIG_MMA8X5X_I2C_ADDRESS
#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void)
{
    int res;
    nvs_handle_t my_handle;
    char state[2] = "0";

    // Initialize the MMA8x5x accelerometer
    res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("MMA8x5x initialization failed: %d\n", res);
        return 1;
    }

    // Initialize NVS
    res = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (res != 0) {
        printf("NVS initialization failed: %d\n", res);
        return 1;
    }

    while (1) {
        // Read the current state from NVS
        size_t required_size = sizeof(state);
        res = nvs_get_str(my_handle, NVS_KEY, state, &required_size);
        if (res != 0) {
            printf("Failed to read state from NVS: %d\n", res);
        } else {
            printf("Current state: %s\n", state);
        }

        // Toggle the state
        if (strcmp(state, "0") == 0) {
            strcpy(state, "1");
        } else {
            strcpy(state, "0");
        }

        // Write the new state to NVS
        res = nvs_set_str(my_handle, NVS_KEY, state);
        if (res != 0) {
            printf("Failed to write state to NVS: %d\n", res);
        } else {
            printf("New state written: %s\n", state);
        }

        // Commit the changes to NVS
        res = nvs_commit(my_handle);
        if (res != 0) {
            printf("NVS commit failed: %d\n", res);
        }

        // Wait for 1 second
        sleep(1);
    }

    // Close the NVS handle
    nvs_close(my_handle);

    return 0;
}
