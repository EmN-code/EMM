#include <stdio.h>
#include <string.h>

#include "nvs_flash.h"
#include "nvs.h"

#define STORAGE_NAMESPACE "storage"

void store_device_state(int state) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      // NVS partition was truncated
      // and needs to be erased
      ESP_ERROR_CHECK(nvs_flash_erase());
      // Retry nvs_flash_init
      err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS handle
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    // Write the value to NVS
    err = nvs_set_i32(my_handle, "state", state);
    if (err != ESP_OK) {
        printf("Failed to write state to NVS (%s)!\n", esp_err_to_name(err));
    } else {
        // Commit written value.
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Failed to commit state to NVS (%s)!\n", esp_err_to_name(err));
        } else {
            printf("State %d stored in NVS\n", state);
        }
    }

    // Close NVS handle
    nvs_close(my_handle);
}

int main() {
    int state = 0;
    while(1) {
        // Toggle state between 0 and 1
        state = !state;

        // Store state in NVS
        store_device_state(state);

        // Wait for a second
        sleep(1);
    }
    return 0;
}