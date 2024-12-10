
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <nvs.h>
#include <esp_err.h>
#include <esp_log.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static void update_device_state(nvs_handle_t handle, int state) {
    esp_err_t err = nvs_set_i32(handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Failed to set device state: %s", esp_err_to_name(err));
    } else {
        err = nvs_commit(handle);
        if (err != ESP_OK) {
            ESP_LOGE("NVS", "Failed to commit device state: %s", esp_err_to_name(err));
        }
    }
}

int main(void) {
    esp_err_t err;
    nvs_handle_t handle;
    int device_state = 0;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        err = nvs_flash_erase();
        if (err != ESP_OK) {
            ESP_LOGE("NVS", "Failed to erase NVS flash: %s", esp_err_to_name(err));
            return -1;
        }
        // Retry initialization
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Failed to initialize NVS: %s", esp_err_to_name(err));
        return -1;
    }

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Failed to open NVS handle: %s", esp_err_to_name(err));
        return -1;
    }

    while (1) {
        // Toggle device state
        device_state = (device_state == 0) ? 1 : 0;

        // Update device state in NVS
        update_device_state(handle, device_state);

        // Print current device state
        printf("Device state updated to: %d\n", device_state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS handle
    nvs_close(handle);

    return 0;
}
