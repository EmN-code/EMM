
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_system.h"

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

void store_state(nvs_handle_t handle, int state) {
    esp_err_t err;
    err = nvs_set_i32(handle, NVS_KEY, state);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Error setting state: %s", esp_err_to_name(err));
    } else {
        err = nvs_commit(handle);
        if (err != ESP_OK) {
            ESP_LOGE("NVS", "Error committing state: %s", esp_err_to_name(err));
        }
    }
}

int read_state(nvs_handle_t handle) {
    esp_err_t err;
    int32_t state = 0;
    err = nvs_get_i32(handle, NVS_KEY, &state);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
        ESP_LOGE("NVS", "Error reading state: %s", esp_err_to_name(err));
    }
    return state;
}

int main(void) {
    esp_err_t err;
    nvs_handle_t my_handle;

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
        ESP_LOGE("NVS", "Error opening NVS handle: %s", esp_err_to_name(err));
        return err;
    }

    while (1) {
        // Read the current state from NVS
        int current_state = read_state(my_handle);

        // Toggle the state
        int new_state = (current_state == 1) ? 0 : 1;

        // Store the new state in NVS
        store_state(my_handle, new_state);

        // Print the new state
        ESP_LOGI("NVS", "New state: %d", new_state);

        // Wait for 1 second before repeating
        sleep(1);
    }

    // Close NVS handle
    nvs_close(my_handle);

    return 0;
}
