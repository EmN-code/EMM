
#include <stdio.h>
#include <xtimer.h>
#include <nvs.h>
#include <esp_err.h>
#include <esp_log.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

void update_device_state(nvs_handle_t handle, int state) {
    esp_err_t err = nvs_set_i32(handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Error setting device state: %d", err);
    } else {
        ESP_LOGI("NVS", "Device state updated to: %d", state);
    }
    nvs_commit(handle);
}

int main(void) {
    esp_err_t err;
    nvs_handle_t handle;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Error opening NVS handle: %d", err);
        return -1;
    }

    // Main loop to update device state every second
    int state = 0;
    while (1) {
        update_device_state(handle, state);
        state = !state; // Toggle state
        xtimer_sleep(1); // Sleep for 1 second
    }

    // Close NVS handle
    nvs_close(handle);

    return 0;
}
