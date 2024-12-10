
#include <stdio.h>
#include <string.h>
#include <nvs.h>
#include "xtimer.h"

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

int main(void)
{
    nvs_handle_t my_handle;
    esp_err_t err;
    int device_state = 1; // Initial state

    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("NVS Flash init failed: %d\n", err);
        return err;
    }

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %d\n", err);
        return err;
    }

    while (1) {
        // Toggle the device state
        device_state = (device_state == 1) ? 0 : 1;

        // Store the device state in NVS
        err = nvs_set_i32(my_handle, DEVICE_STATE_KEY, device_state);
        if (err != ESP_OK) {
            printf("NVS set failed: %d\n", err);
            nvs_close(my_handle);
            return err;
        }

        // Commit the changes to NVS
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("NVS commit failed: %d\n", err);
            nvs_close(my_handle);
            return err;
        }

        // Print the current state
        printf("Device state updated to: %d\n", device_state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS
    nvs_close(my_handle);

    return 0;
}
