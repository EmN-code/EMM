#include <stdio.h>
#include <time.h>
#include "nvs.h"
#include "nvs_flash.h"

#define STORAGE_NAMESPACE "storage"
#define DEVICE_STATE_KEY "state"

void store_device_state(int state) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%s) initializing NVS!\n", esp_err_to_name(err));
    } else {
        // Open
        err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
        if (err != ESP_OK) {
            printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        } else {
            // Write
            err = nvs_set_i32(my_handle, DEVICE_STATE_KEY, state);
            if (err != ESP_OK) {
                printf("Error (%s) writing to NVS!\n", esp_err_to_name(err));
            }

            // Commit written value.
            err = nvs_commit(my_handle);
            if (err != ESP_OK) {
                printf("Error (%s) committing to NVS!\n", esp_err_to_name(err));
            }

            // Close
            nvs_close(my_handle);
        }
    }
}

int main(void) {
    int device_state = 0;

    while (1) {
        // Toggle device state
        device_state = !device_state;

        // Store new state
        store_device_state(device_state);

        // Delay 1 second
        sleep(1);
    }

    return 0;
}