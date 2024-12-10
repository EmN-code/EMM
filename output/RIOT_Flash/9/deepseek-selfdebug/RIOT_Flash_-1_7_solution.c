
#include <stdio.h>
#include <unistd.h>
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_system.h"

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

void store_device_state(nvs_handle_t handle, int state) {
    nvs_set_i32(handle, DEVICE_STATE_KEY, state);
    nvs_commit(handle);
}

int main(void) {
    esp_err_t err;
    nvs_handle_t my_handle;
    int state = 1; // Initial state

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        while (1) {
            // Toggle state
            state = (state == 1) ? 0 : 1;

            // Store the state in NVS
            store_device_state(my_handle, state);

            // Print the current state
            printf("Device state stored: %d\n", state);

            // Wait for 1 second
            sleep(1);
        }

        // Close NVS
        nvs_close(my_handle);
    }

    return 0;
}
