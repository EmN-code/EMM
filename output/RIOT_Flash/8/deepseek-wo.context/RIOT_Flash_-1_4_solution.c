
#include <stdio.h>
#include <unistd.h>
#include "nvs.h"
#include "mma8x5x.h"

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"
#define MMA8X5X_I2C_ADDRESS CONFIG_MMA8X5X_I2C_ADDRESS

int main(void)
{
    nvs_handle_t my_handle;
    esp_err_t err;
    int32_t device_state = 0;

    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("NVS Flash initialization failed: %d\n", err);
        return -1;
    }

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("NVS Open failed: %d\n", err);
        return -1;
    }

    // Initialize MMA8X5X accelerometer
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("MMA8X5X initialization failed\n");
        return -1;
    }

    while (1) {
        // Read the current device state from NVS
        err = nvs_get_i32(my_handle, NVS_KEY, &device_state);
        if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
            printf("NVS Get failed: %d\n", err);
            return -1;
        }

        // Toggle the device state
        device_state = (device_state == 0) ? 1 : 0;

        // Update the device state in NVS
        err = nvs_set_i32(my_handle, NVS_KEY, device_state);
        if (err != ESP_OK) {
            printf("NVS Set failed: %d\n", err);
            return -1;
        }

        // Commit the changes to NVS
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("NVS Commit failed: %d\n", err);
            return -1;
        }

        // Print the current device state
        printf("Device state updated to: %ld\n", (long)device_state);

        // Wait for 1 second
        sleep(1);
    }

    // Close NVS
    nvs_close(my_handle);

    return 0;
}
