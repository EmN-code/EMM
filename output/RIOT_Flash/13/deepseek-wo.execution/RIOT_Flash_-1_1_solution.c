
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_log.h"

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

void store_device_state(nvs_handle_t handle, int state) {
    esp_err_t err;
    err = nvs_set_i32(handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Error setting device state: %s", esp_err_to_name(err));
    } else {
        err = nvs_commit(handle);
        if (err != ESP_OK) {
            ESP_LOGE("NVS", "Error committing NVS: %s", esp_err_to_name(err));
        }
    }
}

int read_device_state(nvs_handle_t handle) {
    int32_t state = 0;
    esp_err_t err = nvs_get_i32(handle, DEVICE_STATE_KEY, &state);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
        ESP_LOGE("NVS", "Error reading device state: %s", esp_err_to_name(err));
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
        // Simulate temperature update (toggle state)
        int current_state = read_device_state(my_handle);
        int new_state = (current_state == 1) ? 0 : 1;
        store_device_state(my_handle, new_state);

        // Print current state
        ESP_LOGI("NVS", "Device state updated to: %d", new_state);

        // Wait for 1 second
        sleep(1);
    }

    // Close NVS handle
    nvs_close(my_handle);

    return 0;
}
