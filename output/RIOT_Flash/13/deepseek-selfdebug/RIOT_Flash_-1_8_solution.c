
#include <stdio.h>
#include <unistd.h>  // for sleep function
#include "nvs.h"     // include the NVS header file

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

int main(void)
{
    nvs_handle_t my_handle;
    int err;
    int device_state = 1;  // Example state to store

    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("NVS Flash initialization failed: %d\n", err);
        return 1;
    }

    // Open NVS namespace
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("NVS Open failed: %d\n", err);
        return 1;
    }

    while (1) {
        // Store the device state in NVS
        err = nvs_set_i32(my_handle, NVS_KEY, device_state);
        if (err != ESP_OK) {
            printf("NVS Set failed: %d\n", err);
        } else {
            printf("Device state stored: %d\n", device_state);
        }

        // Commit the changes to NVS
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("NVS Commit failed: %d\n", err);
        }

        // Toggle the device state
        device_state = (device_state == 1) ? 0 : 1;

        // Wait for 1 second
        sleep(1);
    }

    // Close the NVS handle
    nvs_close(my_handle);

    return 0;
}
