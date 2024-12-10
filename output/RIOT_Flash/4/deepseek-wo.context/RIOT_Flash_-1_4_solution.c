
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "nvs.h"

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

int main(void) {
    nvs_handle_t my_handle;
    esp_err_t err;
    int32_t device_state = 0;

    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("NVS initialization failed: %d\n", err);
        return -1;
    }

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %d\n", err);
        return -1;
    }

    while (1) {
        // Read the current device state from NVS
        err = nvs_get_i32(my_handle, NVS_KEY, &device_state);
        if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
            printf("NVS read failed: %d\n", err);
            nvs_close(my_handle);
            return -1;
        }

        // Toggle the device state
        device_state = (device_state == 0) ? 1 : 0;

        // Write the updated device state back to NVS
        err = nvs_set_i32(my_handle, NVS_KEY, device_state);
        if (err != ESP_OK) {
            printf("NVS write failed: %d\n", err);
            nvs_close(my_handle);
            return -1;
        }

        // Commit the changes to NVS
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("NVS commit failed: %d\n", err);
            nvs_close(my_handle);
            return -1;
        }

        // Print the current device state
        printf("Device state: %ld\n", (long)device_state);

        // Wait for 1 second before repeating
        sleep(1);
    }

    // Close NVS
    nvs_close(my_handle);

    return 0;
}
